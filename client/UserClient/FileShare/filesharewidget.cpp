#include "filesharewidget.h"
#include "ui_filesharewidget.h"

FileShareWidget::FileShareWidget(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::FileShareWidget)
{
	ui->setupUi(this);
	ui->return_message_label->setVisible(false);
}

FileShareWidget::FileShareWidget(FileMetadata *entity, QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::FileShareWidget)
{
	ui->setupUi(this);
	this->entity = entity;
	ui->return_message_label->setVisible(false);
	ui->filename_label->setText(this->entity->getfilename());
}

FileShareWidget::~FileShareWidget()
{
	delete ui;
}

void FileShareWidget::on_confirm_pushButton_clicked()
{
	//    emit file_share_signal(this->entity);
	QString sharedusername = ui->username_lineEdit->text();
	if (nullptr == net_manager) {
		net_manager = new QNetworkAccessManager(this);
		connect(net_manager, SIGNAL(finished(QNetworkReply * )),
				this, SLOT(slot_replyFinished(QNetworkReply * )));
		connect(net_manager, SIGNAL(sslErrors(QNetworkReply * , QList<QSslError>)),
				this, SLOT(slot_sslErrors(QNetworkReply * , QList<QSslError>)));
		connect(net_manager, SIGNAL(authenticationRequired(QNetworkReply * , QAuthenticator * )),
				this, SLOT(slot_provideAuthenication(QNetworkReply * , QAuthenticator * )));
	}
	if (nullptr != sharedusername && sharedusername.length() > 0) {
		QNetworkRequest network_request;
		QSslConfiguration config;
		config.setPeerVerifyMode(QSslSocket::VerifyNone);
		config.setProtocol(QSsl::TlsV1_2);
		network_request.setSslConfiguration(config);
		network_request.setUrl(QUrl(url + "sharedata"));
		network_request.setRawHeader("sessionid", sessionid.toUtf8());
		network_request.setRawHeader("Content-Type", "application/json");
		//    QVariantList varList;
		QVariantMap var;
		var.insert("method", "sharedata");
		var.insert("version", "1.0");
		var.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

		QVariantMap requestvar;
		requestvar.insert("fileuniqueid", this->entity->getfileuniqueid());
		requestvar.insert("sharedusername", sharedusername);
		var.insert("request", requestvar);
		QJsonObject obJct = QJsonObject::fromVariantMap(var);
		QJsonDocument jsonDoc(obJct);
		QByteArray json = jsonDoc.toJson();
		QString messagejsonstr(json);
		//    messagejsonstr = QtJson::serialize(messagejsonobj);
		qDebug() << Q_FUNC_INFO << "messagejsonstr is " << messagejsonstr;
		post_reply = net_manager->post(network_request, messagejsonstr.toUtf8());
		connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
				this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
	}

}

void FileShareWidget::slot_replyFinished(QNetworkReply *reply)
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
		qDebug() << Q_FUNC_INFO << "ret_data is " << ret_data;
	}

}
void FileShareWidget::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	QSslCertificate sslcert = errors[0].certificate();
	reply->ignoreSslErrors();
}
void FileShareWidget::slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator)
{

}
void FileShareWidget::slot_NetWorkError(QNetworkReply::NetworkError)
{
	if (nullptr != post_reply) {
		post_reply->deleteLater();
		post_reply = nullptr;
	}
	if (nullptr != net_manager) {
		delete net_manager;
		net_manager = nullptr;
	}
}

void FileShareWidget::on_cancel_pushButton_clicked()
{
	this->close();
}
