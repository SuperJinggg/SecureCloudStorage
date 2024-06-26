#include <QJsonParseError>
#include <QJsonObject>
#include <qdatetime.h>
#include <qjsonarray.h>
#include <qjsonvalue.h>
#include <qmap.h>
#include <qtimer.h>
#include <qurl.h>
#include <qvariant.h>
#include "systemadminwidget.h"
#include "ui_systemadminwidget.h"
#include "global.h"

SystemAdminWidget::SystemAdminWidget(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::SystemAdminWidget),
	init_endpoints{"health", "metrics/jvm.memory.max", "metrics/jvm.memory.used", "metrics/system.cpu.count", "env"},
	endpoints{"health", "metrics/jvm.memory.used"}
{
	ui->setupUi(this);
	if (nullptr == manager) {
		manager = new QNetworkAccessManager(this);
		connect(manager, SIGNAL(finished(QNetworkReply * )),
				this, SLOT(slot_replyFinished(QNetworkReply * )));
		connect(manager, SIGNAL(sslErrors(QNetworkReply * , QList<QSslError>)),
				this, SLOT(slot_sslErrors(QNetworkReply * , QList<QSslError>)));
		connect(manager, SIGNAL(authenticationRequired(QNetworkReply * , QAuthenticator * )),
				this, SLOT(slot_provideAuthenication(QNetworkReply * , QAuthenticator * )));
	}

	system_audit();
	init = false;

	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &SystemAdminWidget::system_audit);
	timer->start(3000);
}

void SystemAdminWidget::system_audit()
{
	QNetworkRequest request;
	QSslConfiguration config;
	QString Url = url + "monitor/";
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_2);
	request.setSslConfiguration(config);
	QString input = username + ":" + password;
	char *inputbyte = (input.toUtf8().data());
	long inputlen;
	long outputlen;
	char *output = nullptr;
	//计算经过base64编码后的字符串长度
	inputlen = strlen((char *)inputbyte);
	if (inputlen % 3 == 0) {
		outputlen = inputlen / 3 * 4;
	}
	else {
		outputlen = (inputlen / 3 + 1) * 4;
	}
	output = (char *)malloc(sizeof(char) * outputlen + 1);
	output[outputlen] = '\0';
	base64enc(output, inputbyte, inputlen);
	QString authorizationstr = QString("Basic ") + QString(output);
	request.setRawHeader("Authorization", authorizationstr.toUtf8());
	if (init)
		for (auto endpoint: init_endpoints) {
			request.setUrl(QUrl(Url + endpoint));
			post_reply = manager->get(request);
		}
	else
		for (auto endpoint: endpoints) {
			request.setUrl(QUrl(Url + endpoint));
			post_reply = manager->get(request);
		}

	connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
			this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}

void SystemAdminWidget::slot_replyFinished(QNetworkReply *reply)
{
	QString ret_data;
	QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (statusCode.isValid())
		qDebug() << Q_FUNC_INFO << "status code=" << statusCode.toInt();
	QVariantMap details;
	bool ifexist;
	if (nullptr != reply) {
		ret_data = reply->readAll();
		qDebug() << Q_FUNC_INFO << "reply readAll is " << ret_data;

		// 磁盘空间
		if (reply->url().toString().contains("health")) {
			QByteArray resultjsonbytearray;
			QJsonParseError parseresult;
			resultjsonbytearray.append(ret_data);
			QJsonDocument parse_doucment = QJsonDocument::fromJson(resultjsonbytearray, &parseresult);
			if (parseresult.error == QJsonParseError::NoError) {
				if (parse_doucment.isObject()) {
					QVariant total;
					QVariant free;

					details = parse_doucment.toVariant().toMap();
					QMapIterator<QString, QVariant> iterater(details);
					while (iterater.hasNext()) {
						iterater.next();
						QString iteraterkey = iterater.key();
						QVariant iteratervalue = iterater.value();
						if (iteraterkey == "details" || iteraterkey == "diskSpace") {
							details = iteratervalue.toMap();
							iterater = QMapIterator<QString, QVariant>(details);
						}
						else if (iteraterkey == "total") {
							total = iteratervalue;
						}
						else if (iteraterkey == "free") {
							free = iteratervalue;
						}
					}

					ui->listWidget->item(0)->setText("磁盘总空间：" + dataConvert(total.toLongLong()));
					ui->listWidget->item(1)
						->setText("磁盘已使用空间：" + dataConvert(total.toLongLong() - free.toLongLong()));
					ui->listWidget->item(2)->setText("磁盘未使用空间：" + dataConvert(free.toLongLong()));
					double percent = 1 - ((double)free.toLongLong()) / total.toLongLong();
					ui->progressBar->setValue(percent * 100);
				}
			}
		}

			// JVM 内存和 CPU 个数
		else if (reply->url().toString().contains("metrics")) {
			QByteArray resultjsonbytearray;
			QJsonParseError parseresult;
			resultjsonbytearray.append(ret_data);
			QJsonDocument parse_doucment = QJsonDocument::fromJson(resultjsonbytearray, &parseresult);
			if (parseresult.error == QJsonParseError::NoError) {
				if (parse_doucment.isObject()) {
					QVariant mem;
					QJsonValue v = parse_doucment.object().value("measurements");
					mem = v.toArray()[0].toObject().value("value");

					if (reply->url().toString().contains("jvm.memory.max")) {
						maxmem = mem;
						ui->listWidget_2->item(0)->setText("JVM最大内存：" + dataConvert(mem.toLongLong()));
					}

					else if (reply->url().toString().contains("jvm.memory.used")) {
						ui->listWidget_2->item(1)->setText("JVM已使用内存大小: " + dataConvert(mem.toLongLong()));
						if (!maxmem.isNull()) {
							ui->progressBar_2->setValue(100 * (mem.toDouble() / maxmem.toDouble()));
							ui->listWidget_2->item(2)
								->setText("JVM可用内存大小：" + dataConvert(maxmem.toLongLong() - mem.toLongLong()));
						}
					}

					else if (reply->url().toString().contains("system.cpu.count")) {
						ui->listWidget_3->item(0)->setText("系统CPU个数：" + QString::number(mem.toDouble()));
					}
				}
			}
		}

			// 系统信息
		else if (reply->url().toString().contains("env")) {
			QByteArray resultjsonbytearray;
			QJsonParseError parseresult;
			resultjsonbytearray.append(ret_data);
			QJsonDocument parse_doucment = QJsonDocument::fromJson(resultjsonbytearray, &parseresult);
			if (parseresult.error == QJsonParseError::NoError) {
				if (parse_doucment.isObject()) {
					QVariant mem;
					QJsonValue v = parse_doucment.object().value("propertySources");
					for (const auto obj: v.toArray()) {
						if (obj.toObject().value("name") == "server.ports") {
							auto port =
								obj.toObject().value("properties")
									.toObject().value("local.server.port")
									.toObject().value("value");
							ui->listWidget_3->item(3)->setText("端口：" + QString::number(port.toDouble()));
						}

						else if (obj.toObject().value("name") == "systemProperties") {
							auto val =
								obj.toObject().value("properties")
									.toObject().value("java.runtime.version")
									.toObject().value("value");
							ui->listWidget_3->item(1)->setText("JAVA版本：" + val.toString());

							val =
								obj.toObject().value("properties")
									.toObject().value("PID")
									.toObject().value("value");
							ui->listWidget_3->item(4)->setText("进程号：" + val.toString());
						}
					}
				}
			}
		}
	}
}

void SystemAdminWidget::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	QSslCertificate sslcert = errors[0].certificate();
	reply->ignoreSslErrors();
}

void SystemAdminWidget::slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator)
{

}

void SystemAdminWidget::slot_NetWorkError(QNetworkReply::NetworkError errorCode)
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

SystemAdminWidget::~SystemAdminWidget()
{
	delete ui;
}

void SystemAdminWidget::on_system_management_pushButton_clicked()
{
	ui->main_stackedWidget->setCurrentIndex(0);
	system_audit();
	QStandardItemModel *model = new QStandardItemModel(this);
	QStandardItem *item = new QStandardItem("item1");
	model->appendRow(item);
	item = new QStandardItem("item2");
	model->appendRow(item);
//    ui->listView_2->setModel(model);
}

QString SystemAdminWidget::dataConvert(long long int data)
{
	double result = data;
	QString units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
	int cnt = 0;
	while (result > 128) {
		result /= 1024;
		cnt++;
	}
	return QString::number(result, 'g', 3) + units[cnt];
}
