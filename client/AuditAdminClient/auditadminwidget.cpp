#include "auditadminwidget.h"
#include "ui_auditadminwidget.h"
#include "global.h"

AuditAdminWidget::AuditAdminWidget(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::AuditAdminWidget)
{
	ui->setupUi(this);


	ui->start_dateTimeEdit->setCalendarPopup(true);
	ui->end_dateTimeEdit->setCalendarPopup(true);
	ui->start_dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-7));
	ui->end_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
	ui->start_dateTimeEdit->setCalendarPopup(true);
	ui->end_dateTimeEdit->setCalendarPopup(true);
	ui->start_dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-7));
	ui->end_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
	if (nullptr == jumptopage_linedit) {
		jumptopage_linedit = new Local_LineEdit();
		jumptopage_linedit->setValidator(new QIntValidator(1, 10000, this)); //1, 10000为输入的数字值范围
		connect(jumptopage_linedit,
				SIGNAL(signal_linedit_focussed(bool)),
				this, SLOT(slot_audit_jumptopage_linedit_focussed(bool)));

		jumptopage_linedit->setText(QString::number(current_page + 1));
		ui->audit_jumptopage_lineEdit_horizontalLayout->addWidget(jumptopage_linedit);
	}

	ui->lastlogintime_label->setText(lashauthtime);
	ui->lastauth_ipaddress_label->setText(lastauthipaddress);
	ui->lable_current_username->setText(username);

	audit_starttime = ui->start_dateTimeEdit->dateTime();
	audit_endtime = ui->end_dateTimeEdit->dateTime();
	module = ui->opration_audit_search_block_comboBox->currentText();
	search_str = ui->search_username_lineEdit->text();
	current_page = this->jumptopage_linedit->text().toInt();
	show_list_num = ui->audit_show_num_comboBox->currentText().toInt();


	//    ui->operation_audit_record_tableWidget->setAlternatingRowColors(true);
	ui->operation_audit_record_tableWidget->setSortingEnabled(true);
	ui->operation_audit_record_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui->operation_audit_record_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	//    ui->operation_audit_record_tableWidget->sortItems(4);
	ui->operation_audit_record_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
	//默认已某列排序

	sendrequest(2);
}

void AuditAdminWidget::slot_audit_jumptopage_linedit_focussed(bool stat)
{
	jumptopage_lineedit_focus = stat;
}

void AuditAdminWidget::sendrequest(int index)
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
	request.setUrl(QUrl(url + "auditlogsearch"));
	request.setRawHeader("Content-Type", "application/json");
	request.setRawHeader("sessionid", sessionid.toUtf8());

	QVariantMap messagejsonvar;
	messagejsonvar.insert("method", "auditlogsearch");
	messagejsonvar.insert("version", "1.0");
	QVariantMap requestvar;

//	requestvar.insert("auditlogrole", "系统管理员");
	switch (index) {
	case 0:
		requestvar.insert("auditlogrole", "系统管理员");
		break;
	case 1:
		requestvar.insert("auditlogrole", "安全保密管理员");
		break;
	case 2:
		requestvar.insert("auditlogrole", "普通用户");
		break;
	default:
		qDebug() << "error in  auditlogrole";
	}

	requestvar.insert("fromtime", audit_starttime.toString("yyyy-MM-dd hh:mm:ss"));
	requestvar.insert("totime", audit_endtime.toString("yyyy-MM-dd hh:mm:ss"));
	requestvar.insert("user", search_str);
	requestvar.insert("module", module);
	requestvar.insert("pagenum", current_page - 1);
	requestvar.insert("pagesize", show_list_num);
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

void AuditAdminWidget::refreshview(int index)
{
	switch (index) {
	case 0: {
		ui->start_dateTimeEdit->setCalendarPopup(true);
		ui->end_dateTimeEdit->setCalendarPopup(true);
		ui->start_dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-7));
		ui->end_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
		ui->start_dateTimeEdit->setCalendarPopup(true);
		ui->end_dateTimeEdit->setCalendarPopup(true);
		ui->start_dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-7));
		ui->end_dateTimeEdit->setDateTime(QDateTime::currentDateTime());


		ui->operation_audit_record_tableWidget->setRowCount(0);
		ui->operation_audit_record_tableWidget->setRowCount(auditlog->size());
		int addrow = 0;
		if (nullptr != auditlog) {

			QMapIterator<QString, AuditEntity *> list_result_iterater(*auditlog);

			while (list_result_iterater.hasNext()) {

				list_result_iterater.next();
				AuditEntity *list_result_value = list_result_iterater.value();

				QTableWidgetItem *usernameitem = new QTableWidgetItem(list_result_value->getusername());
				ui->operation_audit_record_tableWidget->setItem(addrow, 0, usernameitem);

				QTableWidgetItem *userroleitem = new QTableWidgetItem(list_result_value->getrole());
				ui->operation_audit_record_tableWidget->setItem(addrow, 1, userroleitem);

				QTableWidgetItem *ipaddressitem = new QTableWidgetItem(list_result_value->getipaddress());
				ui->operation_audit_record_tableWidget->setItem(addrow, 2, ipaddressitem);

				QTableWidgetItem *methoditem = new QTableWidgetItem(list_result_value->getmethod());
				ui->operation_audit_record_tableWidget->setItem(addrow, 3, methoditem);

				QTableWidgetItem
					*timeitem = new QTableWidgetItem(list_result_value->gettime().toString("yyyy-MM-dd HH:mm:ss"));
				ui->operation_audit_record_tableWidget->setItem(addrow, 4, timeitem);

				QTableWidgetItem *moduleitem = new QTableWidgetItem(list_result_value->getmodule());
				ui->operation_audit_record_tableWidget->setItem(addrow, 5, moduleitem);
				QTableWidgetItem *objectitem = new QTableWidgetItem(list_result_value->getobject());
				ui->operation_audit_record_tableWidget->setItem(addrow, 6, objectitem);
				QTableWidgetItem *resultitem = new QTableWidgetItem(list_result_value->getresult());
				ui->operation_audit_record_tableWidget->setItem(addrow, 7, resultitem);
				QTableWidgetItem *statusitem = new QTableWidgetItem(list_result_value->getstatus());
				ui->operation_audit_record_tableWidget->setItem(addrow, 8, statusitem);
				ui->operation_audit_record_tableWidget->setColumnWidth(0, 100);
				ui->operation_audit_record_tableWidget->setColumnWidth(1, 100);
				ui->operation_audit_record_tableWidget->setColumnWidth(2, 150);
				ui->operation_audit_record_tableWidget->setColumnWidth(3, 150);
				ui->operation_audit_record_tableWidget->setColumnWidth(4, 150);
				ui->operation_audit_record_tableWidget->setColumnWidth(5, 150);
				ui->operation_audit_record_tableWidget->setColumnWidth(6, 150);
				ui->operation_audit_record_tableWidget->setColumnWidth(7, 150);
				ui->operation_audit_record_tableWidget->setColumnWidth(8, 150);
				ui->operation_audit_record_tableWidget->setRowHeight(addrow, 38);
				//            }
				addrow++;
			}
			ui->operation_audit_record_tableWidget->sortByColumn(4, Qt::DescendingOrder);

			int theendnumber = (current_page) * show_list_num;
			if (theendnumber > totalauditlognumber) {
				theendnumber = totalauditlognumber;
			}

			ui->audit_totalnum_label->setText(
				"显示" + QString::number((current_page - 1) * show_list_num + 1)
					+ "-" + QString::number(theendnumber) + "，共"
					+ QString::number(totalauditlognumber) + "条记录");
			int total_page_audit_search = ((totalauditlognumber) / show_list_num) + 1;

			ui->all_audit_page_total_num_label->setText("共" + QString::number(total_page_audit_search) + "页");

			jumptopage_linedit->setText(QString::number(current_page));

			if (current_page - 1 <= 0) {
				ui->the_left_pushButton->setEnabled(false);
				ui->to_left_pushButton->setEnabled(false);
			}
			else {
				ui->the_left_pushButton->setEnabled(true);
				ui->to_left_pushButton->setEnabled(true);
			}

			if (current_page - 1 >= total_page_audit_search - 1) {
				ui->the_right_pushButton->setEnabled(false);
				ui->to_right_pushButton->setEnabled(false);
			}
			else {
				ui->the_right_pushButton->setEnabled(true);
				ui->to_right_pushButton->setEnabled(true);
			}
		}
	}
		break;
	case 1: {
		ui->securityadmin_audit_start_dateTimeEdit->setCalendarPopup(true);
		ui->end_dateTimeEdit->setCalendarPopup(true);
		ui->securityadmin_audit_start_dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-7));
		ui->end_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
		ui->securityadmin_audit_start_dateTimeEdit->setCalendarPopup(true);
		ui->end_dateTimeEdit->setCalendarPopup(true);
		ui->securityadmin_audit_start_dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-7));
		ui->end_dateTimeEdit->setDateTime(QDateTime::currentDateTime());


		ui->operation_audit_record_tableWidget_2->setRowCount(0);
		ui->operation_audit_record_tableWidget_2->setRowCount(auditlog->size());
		int addrow = 0;
		if (nullptr != auditlog) {

			QMapIterator<QString, AuditEntity *> list_result_iterater(*auditlog);

			while (list_result_iterater.hasNext()) {

				list_result_iterater.next();
				AuditEntity *list_result_value = list_result_iterater.value();

				QTableWidgetItem *usernameitem = new QTableWidgetItem(list_result_value->getusername());
				ui->operation_audit_record_tableWidget_2->setItem(addrow, 0, usernameitem);

				QTableWidgetItem *userroleitem = new QTableWidgetItem(list_result_value->getrole());
				ui->operation_audit_record_tableWidget_2->setItem(addrow, 1, userroleitem);

				QTableWidgetItem *ipaddressitem = new QTableWidgetItem(list_result_value->getipaddress());
				ui->operation_audit_record_tableWidget_2->setItem(addrow, 2, ipaddressitem);

				QTableWidgetItem *methoditem = new QTableWidgetItem(list_result_value->getmethod());
				ui->operation_audit_record_tableWidget_2->setItem(addrow, 3, methoditem);

				QTableWidgetItem
					*timeitem = new QTableWidgetItem(list_result_value->gettime().toString("yyyy-MM-dd HH:mm:ss"));
				ui->operation_audit_record_tableWidget_2->setItem(addrow, 4, timeitem);

				QTableWidgetItem *moduleitem = new QTableWidgetItem(list_result_value->getmodule());
				ui->operation_audit_record_tableWidget_2->setItem(addrow, 5, moduleitem);
				QTableWidgetItem *objectitem = new QTableWidgetItem(list_result_value->getobject());
				ui->operation_audit_record_tableWidget_2->setItem(addrow, 6, objectitem);
				QTableWidgetItem *resultitem = new QTableWidgetItem(list_result_value->getresult());
				ui->operation_audit_record_tableWidget_2->setItem(addrow, 7, resultitem);
				QTableWidgetItem *statusitem = new QTableWidgetItem(list_result_value->getstatus());
				ui->operation_audit_record_tableWidget_2->setItem(addrow, 8, statusitem);
				ui->operation_audit_record_tableWidget_2->setColumnWidth(0, 100);
				ui->operation_audit_record_tableWidget_2->setColumnWidth(1, 100);
				ui->operation_audit_record_tableWidget_2->setColumnWidth(2, 150);
				ui->operation_audit_record_tableWidget_2->setColumnWidth(3, 150);
				ui->operation_audit_record_tableWidget_2->setColumnWidth(4, 150);
				ui->operation_audit_record_tableWidget_2->setColumnWidth(5, 150);
				ui->operation_audit_record_tableWidget_2->setColumnWidth(6, 150);
				ui->operation_audit_record_tableWidget_2->setColumnWidth(7, 150);
				ui->operation_audit_record_tableWidget_2->setColumnWidth(8, 150);
				ui->operation_audit_record_tableWidget_2->setRowHeight(addrow, 38);
				//            }
				addrow++;
			}
			ui->operation_audit_record_tableWidget_2->sortByColumn(4, Qt::DescendingOrder);

			int theendnumber = (current_page) * show_list_num;
			if (theendnumber > totalauditlognumber) {
				theendnumber = totalauditlognumber;
			}

			ui->audit_totalnum_label_2->setText(
				"显示" + QString::number((current_page - 1) * show_list_num + 1)
					+ "-" + QString::number(theendnumber) + "，共"
					+ QString::number(totalauditlognumber) + "条记录");
			int total_page_audit_search = ((totalauditlognumber) / show_list_num) + 1;

			ui->all_audit_page_total_num_label_2->setText("共" + QString::number(total_page_audit_search) + "页");

			jumptopage_linedit->setText(QString::number(current_page));

			if (current_page - 1 <= 0) {
				ui->the_left_pushButton_2->setEnabled(false);
				ui->to_left_pushButton_2->setEnabled(false);
			}
			else {
				ui->the_left_pushButton_2->setEnabled(true);
				ui->to_left_pushButton_2->setEnabled(true);
			}

			if (current_page - 1 >= total_page_audit_search - 1) {
				ui->the_right_pushButton->setEnabled(false);
				ui->to_right_pushButton->setEnabled(false);
			}
			else {
				ui->the_right_pushButton_2->setEnabled(true);
				ui->to_right_pushButton_2->setEnabled(true);
			}
		}
	}
		break;
	case 2: {
		ui->start_dateTimeEdit_2->setCalendarPopup(true);
		ui->end_dateTimeEdit->setCalendarPopup(true);
		ui->start_dateTimeEdit_2->setDateTime(QDateTime::currentDateTime().addDays(-7));
		ui->end_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
		ui->start_dateTimeEdit_2->setCalendarPopup(true);
		ui->end_dateTimeEdit->setCalendarPopup(true);
		ui->start_dateTimeEdit_2->setDateTime(QDateTime::currentDateTime().addDays(-7));
		ui->end_dateTimeEdit->setDateTime(QDateTime::currentDateTime());


		ui->operation_audit_record_tableWidget_3->setRowCount(0);
		ui->operation_audit_record_tableWidget_3->setRowCount(auditlog->size());
		int addrow = 0;
		if (nullptr != auditlog) {

			QMapIterator<QString, AuditEntity *> list_result_iterater(*auditlog);

			while (list_result_iterater.hasNext()) {

				list_result_iterater.next();
				AuditEntity *list_result_value = list_result_iterater.value();

				QTableWidgetItem *usernameitem = new QTableWidgetItem(list_result_value->getusername());
				ui->operation_audit_record_tableWidget_3->setItem(addrow, 0, usernameitem);

				QTableWidgetItem *userroleitem = new QTableWidgetItem(list_result_value->getrole());
				ui->operation_audit_record_tableWidget_3->setItem(addrow, 1, userroleitem);

				QTableWidgetItem *ipaddressitem = new QTableWidgetItem(list_result_value->getipaddress());
				ui->operation_audit_record_tableWidget_3->setItem(addrow, 2, ipaddressitem);

				QTableWidgetItem *methoditem = new QTableWidgetItem(list_result_value->getmethod());
				ui->operation_audit_record_tableWidget_3->setItem(addrow, 3, methoditem);

				QTableWidgetItem
					*timeitem = new QTableWidgetItem(list_result_value->gettime().toString("yyyy-MM-dd HH:mm:ss"));
				ui->operation_audit_record_tableWidget_3->setItem(addrow, 4, timeitem);

				QTableWidgetItem *moduleitem = new QTableWidgetItem(list_result_value->getmodule());
				ui->operation_audit_record_tableWidget_3->setItem(addrow, 5, moduleitem);
				QTableWidgetItem *objectitem = new QTableWidgetItem(list_result_value->getobject());
				ui->operation_audit_record_tableWidget_3->setItem(addrow, 6, objectitem);
				QTableWidgetItem *resultitem = new QTableWidgetItem(list_result_value->getresult());
				ui->operation_audit_record_tableWidget_3->setItem(addrow, 7, resultitem);
				QTableWidgetItem *statusitem = new QTableWidgetItem(list_result_value->getstatus());
				ui->operation_audit_record_tableWidget_3->setItem(addrow, 8, statusitem);
				ui->operation_audit_record_tableWidget_3->setColumnWidth(0, 100);
				ui->operation_audit_record_tableWidget_3->setColumnWidth(1, 100);
				ui->operation_audit_record_tableWidget_3->setColumnWidth(2, 150);
				ui->operation_audit_record_tableWidget_3->setColumnWidth(3, 150);
				ui->operation_audit_record_tableWidget_3->setColumnWidth(4, 150);
				ui->operation_audit_record_tableWidget_3->setColumnWidth(5, 150);
				ui->operation_audit_record_tableWidget_3->setColumnWidth(6, 150);
				ui->operation_audit_record_tableWidget_3->setColumnWidth(7, 150);
				ui->operation_audit_record_tableWidget_3->setColumnWidth(8, 150);
				ui->operation_audit_record_tableWidget_3->setRowHeight(addrow, 38);
				//            }
				addrow++;
			}
			ui->operation_audit_record_tableWidget_3->sortByColumn(4, Qt::DescendingOrder);

			int theendnumber = (current_page) * show_list_num;
			if (theendnumber > totalauditlognumber) {
				theendnumber = totalauditlognumber;
			}

			ui->audit_totalnum_label_3->setText(
				"显示" + QString::number((current_page - 1) * show_list_num + 1)
					+ "-" + QString::number(theendnumber) + "，共"
					+ QString::number(totalauditlognumber) + "条记录");
			int total_page_audit_search = ((totalauditlognumber) / show_list_num) + 1;

			ui->all_audit_page_total_num_label_3->setText("共" + QString::number(total_page_audit_search) + "页");

			jumptopage_linedit->setText(QString::number(current_page));

			if (current_page - 1 <= 0) {
				ui->the_left_pushButton_3->setEnabled(false);
				ui->to_left_pushButton_3->setEnabled(false);
			}
			else {
				ui->the_left_pushButton_3->setEnabled(true);
				ui->to_left_pushButton_3->setEnabled(true);
			}

			if (current_page - 1 >= total_page_audit_search - 1) {
				ui->the_right_pushButton_3->setEnabled(false);
				ui->to_right_pushButton_3->setEnabled(false);
			}
			else {
				ui->the_right_pushButton_3->setEnabled(true);
				ui->to_right_pushButton_3->setEnabled(true);
			}
		}
	}
		break;
	default:
		qDebug() << "refreshview error!";
	}
}

void AuditAdminWidget::slot_replyFinished(QNetworkReply *reply)
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
		QVariantMap details;
		QString username;
		QString auditlogrole;

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
							if (detailsiteraterkey == "auditlogrole") {
								auditlogrole = detailsiteratervalue.toString();
							}
							else if (detailsiteraterkey == "auditlognumber") {
								totalauditlognumber = detailsiteratervalue.toInt();
							}
							else if (detailsiteraterkey == "auditlog") {


								if (nullptr != auditlog) {
									QMapIterator<QString, AuditEntity *> return_result_iterater(*auditlog);
									if (auditlog->size() > 0) {
										while (return_result_iterater.hasNext()) {
											return_result_iterater.next();
											QString iteraterkey = return_result_iterater.key();
											AuditEntity *return_result_value = return_result_iterater.value();
											delete return_result_value;
											return_result_value = NULL;
										}
									}
									delete auditlog;
									auditlog = nullptr;
								}

								if (nullptr == auditlog) {
									auditlog = new QMap<QString, AuditEntity *>();
								}


								QVariantList auditloglist = detailsiteratervalue.toList();
								if (auditloglist.size() > 0) {
									int id;
									QDateTime time; //时间
									QString ipaddress;//客户端IP地质
									QString username;//用户名
									QString role;//用户角色
									QString method;//操作方法,例如账户登录,账户退出,文件上传
									QString module;//模块,登录模块,文件浏览模块
									QString object;//访问资源,目标等,登录模块是xxxx端,文件上传是xxx文件
									QString result;//成功或者失败
									QString status;

									for (int i = 0; i < auditloglist.length(); i++) {
										AuditEntity *auditentity = new AuditEntity();
										QVariantMap auditloglistmap = auditloglist[i].toMap();
										QMapIterator<QString, QVariant> mapiterater(auditloglistmap);
										while (mapiterater.hasNext()) {
											mapiterater.next();
											QString mapkey = mapiterater.key();
											QVariant mapvalue = mapiterater.value();
											if (0 == QString::compare(mapkey, "id")) {
												id = mapvalue.toInt();
											}
											else if (0 == QString::compare(mapkey, "time")) {
												time = mapvalue.toDateTime();
											}
											else if (0 == QString::compare(mapkey, "ipaddress")) {
												ipaddress = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "username")) {
												username = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "role")) {
												role = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "method")) {
												method = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "module")) {
												module = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "object")) {
												object = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "result")) {
												result = mapvalue.toString();
											}
											else if (0 == QString::compare(mapkey, "status")) {
												status = mapvalue.toString();
											}
										}

										auditentity->setid(id);
										auditentity->settime(time); //时间
										auditentity->setipaddress(ipaddress);//客户端IP地质
										auditentity->setusername(username);//用户名
										auditentity->setrole(role);//用户角色
										auditentity->setmethod(method);//操作方法,例如账户登录,账户退出,文件上传
										auditentity->setmodule(module);//模块,登录模块,文件浏览模块
										auditentity->setobject(object);//访问资源,目标等,登录模块是xxxx端,文件上传是xxx文件
										auditentity->setresult(result);//成功或者失败
										auditentity->setstatus(status);
										if (nullptr != auditlog) {
											auditlog->insert(QString(id), auditentity);
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
		qDebug() << Q_FUNC_INFO << "username is " << username;
		qDebug() << Q_FUNC_INFO << "auditlog size is " << auditlog->size();
		qDebug() << "auditlogrole is: " << auditlogrole;

		if (!QString::compare(auditlogrole, "普通用户")) {
			refreshview(2);
			qDebug() << "reflash putongyonghu";
		}
		else if (!QString::compare(auditlogrole, "安全保密管理员")) {
			refreshview(1);
			qDebug() << "reflash anquanbaomi";
		}
		else {
			refreshview(0);
			qDebug() << "reflash xitongguanliyuan";
		}
	}
}
void AuditAdminWidget::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	QSslCertificate sslcert = errors[0].certificate();
	reply->ignoreSslErrors();
}

void AuditAdminWidget::slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator)
{

}

void AuditAdminWidget::slot_NetWorkError(QNetworkReply::NetworkError errorCode)
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

AuditAdminWidget::~AuditAdminWidget()
{
	delete ui;
}

void AuditAdminWidget::on_tabWidget_currentChanged(int index)
{
	sendrequest(index);
}


