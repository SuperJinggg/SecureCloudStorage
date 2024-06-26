#include "onlineupdatedialog.h"
#include "ui_onlineupdatedialog.h"

OnlineUpdateDialog::OnlineUpdateDialog(QWidget *parent)
	:
	QDialog(parent),
	ui(new Ui::OnlineUpdateDialog)
{
	ui->setupUi(this);
	ui->progressBar->setVisible(false);
}

void OnlineUpdateDialog::get_version()
{
	if (nullptr == manager) {
		manager = new QNetworkAccessManager();
		connect(manager, SIGNAL(finished(QNetworkReply * )),
				this, SLOT(slot_replyFinished(QNetworkReply * )));
		connect(manager, SIGNAL(sslErrors(QNetworkReply * , QList<QSslError>)),
				this, SLOT(slot_sslErrors(QNetworkReply * , QList<QSslError>)));
		connect(manager, SIGNAL(authenticationRequired(QNetworkReply * , QAuthenticator * )),
				this, SLOT(slot_provideAuthenication(QNetworkReply * , QAuthenticator * )));
	}
	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_2);
	request.setSslConfiguration(config);
	request.setUrl(QUrl(url + "getcurrentversion"));
	QVariantMap var;
	var.insert("method", "getcurrentversion");
	var.insert("version", "1.0");
	var.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
	QJsonObject obJct = QJsonObject::fromVariantMap(var);
	QJsonDocument jsonDoc(obJct);
	QByteArray json = jsonDoc.toJson();
	QString messagejsonstr(json);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	post_reply = manager->post(request, messagejsonstr.toUtf8());
	connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
			this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}

void OnlineUpdateDialog::slot_replyFinished(QNetworkReply *reply)
{
	QByteArray ret_data;
	QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (statusCode.isValid())
		qDebug() << Q_FUNC_INFO << "status code=" << statusCode.toInt();
	QString method = "";
	QString result = "";
	QString code = "";

	QString message = "";
	QVariantMap details;
	bool ifexist;
	if (nullptr != reply) {
		ret_data = reply->readAll();
#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_
		QString logmessage;
		QTextStream logmessageout(&logmessage);
		logmessageout << ret_data;
		SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage, 1);
#endif
		QString retmethod;
		QString retresult;
		QString retmessage;
		int retcode;

		//解析json
		QByteArray jsonbytearray;
		jsonbytearray.append(ret_data);
		QJsonParseError json_error;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonbytearray, &json_error));
		if (json_error.error != QJsonParseError::NoError) {
			qDebug() << Q_FUNC_INFO << "json error!";
			return;
		}
		QJsonObject rootObj = jsonDoc.object();
		if (rootObj.contains("method")) {
			retmethod = rootObj.value("method").toString();
		}
		if (rootObj.contains("result")) {
			retresult = rootObj.value("result").toString();
		}
		if (rootObj.contains("code")) {
			retcode = rootObj.value("code").toInt();
		}
		if (rootObj.contains("message")) {
			retmessage = rootObj.value("message").toString();
		}

		QJsonObject detailsobj;
		if (rootObj.contains("details")) {
			detailsobj = rootObj.value("details").toObject();
		}
		float version;
		if (detailsobj.contains("currentversion")) {
			version = detailsobj.value("currentversion").toDouble();
		}
		if (NULL != version && version > currentversion) {
			this->setWindowModality(Qt::ApplicationModal);
			this->show();
		}
	}

}

void OnlineUpdateDialog::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	QSslCertificate sslcert = errors[0].certificate();
	reply->ignoreSslErrors();
}

void OnlineUpdateDialog::slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator)
{

}

OnlineUpdateDialog::~OnlineUpdateDialog()
{
	delete ui;
}
void OnlineUpdateDialog::slot_NetWorkError(QNetworkReply::NetworkError)
{
	if (nullptr != post_reply) {
		post_reply->deleteLater();
		post_reply = nullptr;
	}
	if (nullptr != manager) {
		delete manager;
		manager = nullptr;
	}
}

void OnlineUpdateDialog::on_cancel_btn_clicked()
{
	this->close();
}

void OnlineUpdateDialog::on_update_btn_clicked()
{
	if (nullptr == onlineupdateoperation) {
		onlineupdateoperation = new OnlineUpdateOperation();
		connect(this, SIGNAL(signal_updateopeartion_begin()),
				onlineupdateoperation, SLOT(slot_opeartion()));

		connect(onlineupdateoperation, SIGNAL(toSetProgressBar(int)),
				this, SLOT(slot_setprogressbar(int)));
	}

	if (nullptr == updatethread) {
		updatethread = new QThread();
	}
	if (nullptr != updatethread) {
		onlineupdateoperation->DoSetup(*updatethread);
		onlineupdateoperation->moveToThread(updatethread);
	}
	else {

	}

	if (!updatethread->isRunning()) {
		updatethread->start();
	}
	else {
		//send signal
		emit signal_updateopeartion_begin();
	}

}
void OnlineUpdateDialog::slot_setprogressbar(int progress)
{
	ui->progressBar->setVisible(true);
	ui->progressBar->setValue(progress);
}
