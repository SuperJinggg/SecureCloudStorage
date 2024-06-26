#include "securityadminwidget.h"
#include "ui_securityadminwidget.h"
#include "QDebug"
#include "global.h"

SecurityAdminWidget::SecurityAdminWidget(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::SecurityAdminWidget)
{
	ui->setupUi(this);

	ui->normaluser_tableWidget->setSortingEnabled(true);
	ui->normaluser_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui->normaluser_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	//    ui->normaluser_tableWidget->sortItems(7);
	ui->normaluser_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
}

void SecurityAdminWidget::securityadmin_userlist(int index)
{

	QString retdate = "";
	if (nullptr != userentity_list) {
		QMapIterator<QString, UserEntityWidget *> return_result_iterater(*userentity_list);
		if (userentity_list->size() > 0) {
			while (return_result_iterater.hasNext()) {
				return_result_iterater.next();
				QString iteraterkey = return_result_iterater.key();
				UserEntityWidget *return_result_value = return_result_iterater.value();
				delete return_result_value;
				return_result_value = nullptr;
			}
		}
		delete userentity_list;
		userentity_list = nullptr;
	}

	if (nullptr == userentity_list) {
		userentity_list = new QMap<QString, UserEntityWidget *>();
	}

	//发送获取用户标识信息并解析
	//sendrequest("普通用户");
	if (index == 0) {
		sendrequest("普通用户");
	}
	else if (index == 1) {
		sendrequest("系统管理员");
	}
	else if (index == 2) {
		sendrequest("安全保密管理员");
	}
	else if (index == 3) {
		sendrequest("安全审计员");
	}
}

void SecurityAdminWidget::sendrequest(QString role)
{
	if (nullptr == manager) {
		manager = new QNetworkAccessManager(this);
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

	request.setUrl(QUrl(url + "userlist"));
	request.setRawHeader("Content-Type", "application/json");
	request.setRawHeader("sessionid", sessionid.toUtf8());

	QVariantMap messagejsonvar;
	messagejsonvar.insert("method", "userlist");
	messagejsonvar.insert("version", "1.0");
	QVariantMap requestvar;
	requestvar.insert("role", role);
	messagejsonvar.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
	messagejsonvar.insert("request", requestvar);
	QJsonObject obJct = QJsonObject::fromVariantMap(messagejsonvar);
	QJsonDocument jsonDoc(obJct);
	QByteArray json = jsonDoc.toJson();
	QString messagejsonstr(json);
	qDebug() << Q_FUNC_INFO << "messagejsonstr is " << messagejsonstr;

	post_reply = manager->post(request, messagejsonstr.toUtf8());
	connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
			this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}

void SecurityAdminWidget::refresh_view(QTableWidget *qtablewidget)
{
	QTableWidget *current = qtablewidget;

	qDebug() << qtablewidget;
	qDebug() << ui->normaluser_tableWidget;
	current->setRowCount(0);
	current->setRowCount(userentity_list->size());
	int addrow = 0;
	if (nullptr != userentity_list) {
		QMapIterator<QString, UserEntityWidget *> iterater(*userentity_list);
		while (iterater.hasNext()) {
			iterater.next();
			UserEntityWidget *list_result_value = iterater.value();
			current->setItem(addrow, 0, list_result_value->getqcheckbox());
			current->setCellWidget(addrow, 1, list_result_value);
			QTableWidgetItem *username = new QTableWidgetItem(list_result_value->getusername());
			current->setItem(addrow, 2, username);
			QTableWidgetItem *role = new QTableWidgetItem(list_result_value->getrole());
			current->setItem(addrow, 3, role);

			QTableWidgetItem *state = new QTableWidgetItem(list_result_value->getstate());
			current->setItem(addrow, 4, state);


			QTableWidgetItem *registertime = new QTableWidgetItem(list_result_value->getregistertime());
			current->setItem(addrow, 5, registertime);

			current->setColumnWidth(0, 40);  //设置列宽
			current->setColumnWidth(1, 100);
			current->setColumnWidth(2, 130);
			current->setColumnWidth(3, 120);
			current->setColumnWidth(4, 120);
			current->setColumnWidth(5, 200);
			current->setRowHeight(addrow, 38);
			addrow++;
		}
	}
}

void SecurityAdminWidget::slot_replyFinished(QNetworkReply *reply)
{

	QString ret_data;
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
		qDebug() << Q_FUNC_INFO << "reply readAll is " << ret_data;

		QString method = "";
		QString result = "";
		QString code = "";
		QString message = "";
		QString currentrole = "";
		QVariantMap details;

		QByteArray resultjsonbytearray;
		resultjsonbytearray.append(ret_data);
		QJsonParseError parseresult;
		QJsonDocument parse_doucment = QJsonDocument::fromJson(resultjsonbytearray, &parseresult);
		if (parseresult.error == QJsonParseError::NoError) {
			if (parse_doucment.isObject()) {
				QVariantMap parsemap = parse_doucment.toVariant().toMap();
				QMapIterator<QString, QVariant> iterater(parsemap);
				while (iterater.hasNext()) {
					iterater.next();
					QString iteraterkey = iterater.key();
					QVariant iteratervalue = iterater.value();
					if (iteraterkey == "method") {
						method = iteratervalue.toString();
					}
					else if (iteraterkey == "result") {
						result = iteratervalue.toString();
					}
					else if (iteraterkey == "code") {
						code = iteratervalue.toString();
					}
					else if (iteraterkey == "message") {
						message = iteratervalue.toString();
					}
					else if (iteraterkey == "details") {
						details = iteratervalue.toMap();
						QMapIterator<QString, QVariant> detailsiterater(details);
						while (detailsiterater.hasNext()) {
							detailsiterater.next();
							QString detailsiteraterkey = detailsiterater.key();
							QVariant detailsiteratervalue = detailsiterater.value();
							if (detailsiteraterkey == "usernumber") {
								usernumber = detailsiteratervalue.toInt();
							}
							else if (detailsiteraterkey == "userentity_list") {

								if (nullptr != userentity_list) {
									QMapIterator<QString, UserEntityWidget *> return_result_iterater(*userentity_list);
									if (userentity_list->size() > 0) {
										while (return_result_iterater.hasNext()) {
											return_result_iterater.next();
											QString iteraterkey = return_result_iterater.key();
											UserEntityWidget *return_result_value = return_result_iterater.value();
											delete return_result_value;
											return_result_value = nullptr;
										}
									}
									delete userentity_list;
									userentity_list = nullptr;
								}

								if (nullptr == userentity_list) {
									userentity_list = new QMap<QString, UserEntityWidget *>();
								}


								QVariantList userlist = detailsiteratervalue.toList();
								if (userlist.size() > 0) {
									int id;
									QString registertime; //时间
									QString forbidtime;//客户端IP地质
									QString username;//用户名
									QString role;//用户角色
									QString state;

									for (int i = 0; i < userlist.length(); i++) {
										UserEntityWidget *userentity = new UserEntityWidget();
										connect(userentity, SIGNAL(refresh(QString)), this, SLOT(sendrequest(QString)));
										QVariantMap auditloglistmap = userlist[i].toMap();
										QMapIterator<QString, QVariant> mapiterater(auditloglistmap);
										while (mapiterater.hasNext()) {
											mapiterater.next();
											QString mapkey = mapiterater.key();
											QVariant mapvalue = mapiterater.value();
											if (0 == QString::compare(mapkey, "id")) {
												id = mapvalue.toInt();
											}
											else if (0 == QString::compare(mapkey, "registertime")) {
												registertime = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "forbidtime")) {
												forbidtime = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "username")) {
												username = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "role")) {
												role = mapvalue.toString();
												if (currentrole == "")
													currentrole = role;
											}
											else if (0 == QString::compare(mapkey, "state")) {
												state = mapvalue.toString();
											}
										}

										userentity->setid(id);
										userentity->setregistertime(registertime); //时间
										userentity->setforbid_time(forbidtime);//客户端IP地质
										userentity->setusername(username);//用户名
										userentity->setrole(role);//用户角色
										userentity->setstate(state);

										if (nullptr != userentity_list) {
											userentity_list->insert(QString(id), userentity);
										}
									}
								}
							}
						}
					}
				}

			}
		}
		else {
			qDebug() << Q_FUNC_INFO << "解析json失败";

		}

		qDebug() << Q_FUNC_INFO << "method is " << method;
		qDebug() << Q_FUNC_INFO << "result is " << result;
		qDebug() << Q_FUNC_INFO << "code is " << code;
		qDebug() << Q_FUNC_INFO << "message is " << message;
		qDebug() << Q_FUNC_INFO << "userentity size is " << userentity_list->size();

		if (0 == QString::compare(currentrole, "普通用户")) {
			refresh_view(ui->normaluser_tableWidget);
		}
		else if (0 == QString::compare(currentrole, "系统管理员")) {
			refresh_view(ui->datamanager_tableWidget);
		}
		else if (0 == QString::compare(currentrole, "安全保密管理员")) {
			refresh_view(ui->usermanager_tableWidget);
		}
		else if (0 == QString::compare(currentrole, "安全审计员")) {
			refresh_view(ui->usermanager_tableWidget_2);
		}
	}

}

void SecurityAdminWidget::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	QSslCertificate sslcert = errors[0].certificate();
	reply->ignoreSslErrors();
}

void SecurityAdminWidget::slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator)
{

}

void SecurityAdminWidget::slot_NetWorkError(QNetworkReply::NetworkError errorCode)
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

SecurityAdminWidget::~SecurityAdminWidget()
{
	delete ui;
}

void SecurityAdminWidget::on_pushButton_usermanagement_clicked()
{
	ui->stackedWidget->setCurrentIndex(1);
	ui->pushButton_usermanagement->
		setStyleSheet(
		"QPushButton#pushButton_usermanagement{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebarimg/yonghu_en.png);"
		"}"
		"QPushButton#pushButton_usermanagement:hover{"
		"background-image: url(:/pic/resource/img/titlebarimg/yonghu_hua.png);"
		"}"
		"QPushButton#pushButton_usermanagement:pressed{"
		"background-image: url(:/pic/resource/img/titlebarimg/yonghu_en.png);"
		"}"
	);

}

void SecurityAdminWidget::on_pushButton_operation_audit_clicked()
{
	ui->stackedWidget->setCurrentIndex(0);

	ui->pushButton_usermanagement->
		setStyleSheet(
		"QPushButton#pushButton_usermanagement{border-radius:0px;background-image: url(:/pic/resource/img/titlebarimg/yonghu_mo.png);}QPushButton#pushButton_usermanagement:hover{background-image: url(:/pic/resource/img/titlebarimg/yonghu_hua.png);}QPushButton#pushButton_usermanagement:pressed{background-image: url(:/pic/resource/img/titlebarimg/yonghu_en.png);}"
		//                "background-image: url(:/pic/resource/img/btn3/titlebar/yonghu_mo.png);"

	);
}

void SecurityAdminWidget::on_add_securityadmin_pushButton_clicked()
{
	role="普通用户";
	w.show();
}

void SecurityAdminWidget::on_auditadmin_tabWidget_currentChanged(int index)
{
	securityadmin_userlist(index);
}

void SecurityAdminWidget::on_delete_usermanager_pushButton_clicked()
{

}
