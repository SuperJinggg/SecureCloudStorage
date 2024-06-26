#include "userregisterwidget.h"
#include "ui_userregisterwidget.h"
#include <qnamespace.h>

UserRegisterWidget::UserRegisterWidget(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::UserRegisterWidget)
{
	setWindowModality(Qt::ApplicationModal);
	ui->setupUi(this);
	if (nullptr == username_lineEdit) {
		username_lineEdit = new Local_LineEdit();//初始化Local_LineEdit
		connect(username_lineEdit,
				SIGNAL(signal_linedit_focussed(bool)),
				this, SLOT(slot_username_linedit_focussed(bool)));//连接信号与槽
		connect(username_lineEdit,
				SIGNAL(textChanged(QString)),
				this, SLOT(slot_username_textChanged(QString)));//连接信号与槽


		username_lineEdit->setText("");//设置输入框默认为空
		ui->username_lineEdit_again_horizontalLayout->addWidget(username_lineEdit);//将username_lineEdit添加到事先摆放好的水平布局中
		
	}
	roleComboBox = ui->roleComboBox;
	this->username_lineEdit->setMinimumHeight(22);//设置输入框最小高度
	this->username_lineEdit->setMinimumWidth(200);//设置输入框最小宽度

	if (nullptr == password1_lineEdit) {
		password1_lineEdit = new Local_LineEdit();
		connect(password1_lineEdit,
				SIGNAL(signal_linedit_focussed(bool)),
				this, SLOT(slot_password1_linedit_focussed(bool)));
		connect(password1_lineEdit,
				SIGNAL(textChanged(QString)),
				this, SLOT(slot_password1_textChanged(QString)));//连接信号与槽

		password1_lineEdit->setText("");
		ui->password1_lineEdit_horizontalLayout->addWidget(password1_lineEdit);

	}
	this->password1_lineEdit->setMinimumHeight(22);
	this->password1_lineEdit->setMinimumWidth(200);
	this->password1_lineEdit->setEchoMode(QLineEdit::Password);//设置口令1输入框的输入模式为口令



	if (nullptr == password2_lineEdit) {
		password2_lineEdit = new Local_LineEdit();
		connect(password2_lineEdit,
				SIGNAL(signal_linedit_focussed(bool)),
				this, SLOT(slot_password2_linedit_focussed(bool)));
		connect(password2_lineEdit,
				SIGNAL(textChanged(QString)),
				this, SLOT(slot_password2_textChanged(QString)));//连接信号与槽
		password2_lineEdit->setText("");
		ui->password2_lineEdit_horizontalLayout->addWidget(password2_lineEdit);

	}
	this->password2_lineEdit->setMinimumHeight(22);
	this->password2_lineEdit->setMinimumWidth(200);
	this->password2_lineEdit->setEchoMode(QLineEdit::Password);//设置口令2输入框的输入模式为口令


	ui->username_notice_label->hide();
	ui->password1_notice_label->hide();
	ui->password2_notice_label->hide();
	ui->caps_notice_label->hide();
	ui->registerresult_label->hide();

}

bool UserRegisterWidget::CapsButtonState()
{

#ifdef Q_OS_WIN// WIN
	if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0) {
		return true;
	}
	else {
		return false;
	}
#endif
#ifdef  Q_OS_LINUX
	QProcess process;
	process.start("bash", QStringList() << "-c" << "xset -q | grep Caps");
	process.waitForFinished();
	QByteArray output;
	output = process.readAllStandardOutput();
	QString str_output = output;
	str_output = str_output.replace(" ","");
	QString state = str_output.mid(str_output.indexOf("CapsLock:",Qt::CaseInsensitive)+9,2);
	if(0==QString::compare(state,"on")){
		return true;
	}
	else if(0==QString::compare(state,"of")){
		return false;
	}else{
	}
#endif
}

void UserRegisterWidget::checkUsernameExistence()
{
	QString username = this->username_lineEdit->text();
	if (nullptr == manager) {
		manager = new QNetworkAccessManager(this);
		connect(manager, SIGNAL(finished(QNetworkReply * )),
				this, SLOT(slot_replyFinished(QNetworkReply * )));
		connect(manager, SIGNAL(sslErrors(QNetworkReply * , QList<QSslError>)),
				this, SLOT(slot_sslErrors(QNetworkReply * , QList<QSslError>)));
		connect(manager, SIGNAL(authenticationRequired(QNetworkReply * , QAuthenticator * )),
				this, SLOT(slot_provideAuthenication(QNetworkReply * , QAuthenticator * )));
	}
	QNetworkRequest network_request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_2);
	network_request.setSslConfiguration(config);
	network_request.setUrl(QUrl(url + "checkusernameexistence"));

	network_request.setRawHeader("Content-Type", "application/json");
	QVariantMap messagejsonvar;

	messagejsonvar.insert("method", "checkusernameexistence");
	messagejsonvar.insert("version", "1.0");
	QVariantMap requestvar;
	requestvar.insert("username", username);
	messagejsonvar.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	messagejsonvar.insert("request", requestvar);
	QJsonObject obJct = QJsonObject::fromVariantMap(messagejsonvar);
	QJsonDocument jsonDoc(obJct);
	QByteArray json = jsonDoc.toJson();
	QString messagejsonstr(json);

	qDebug() << Q_FUNC_INFO << "messagejsonstr is " << messagejsonstr;
	post_reply = manager->post(network_request, messagejsonstr.toUtf8());
	connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
			this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}

void UserRegisterWidget::slot_username_linedit_focussed(bool stat)
{//输入框获取或失去鼠标焦点出发的槽函数
	qDebug() << Q_FUNC_INFO << "stat is " << stat;
	capskeystat = stat;
	if (stat == true) {
		if (CapsButtonState()) {
			ui->caps_notice_label->show();
		}
		else {
			ui->caps_notice_label->hide();
		}
	}
	else {
		ui->caps_notice_label->hide();
		if (checkUsernameRegular()) {
			ui->username_notice_label->hide();
			checkUsernameExistence();
		}
	}

}

void UserRegisterWidget::slot_password1_linedit_focussed(bool stat)
{//输入框获取或失去鼠标焦点出发的槽函数
	qDebug() << Q_FUNC_INFO << "stat is " << stat;
	capskeystat = stat;
	if (stat == true) {
		if (CapsButtonState()) {
			ui->caps_notice_label->show();
		}
		else {
			ui->caps_notice_label->hide();
		}
	}
	else {
		ui->caps_notice_label->hide();
	}

}
void UserRegisterWidget::slot_password2_linedit_focussed(bool stat)
{//输入框获取或失去鼠标焦点出发的槽函数
	qDebug() << Q_FUNC_INFO << "stat is " << stat;
	capskeystat = stat;
	if (stat == true) {
		if (CapsButtonState()) {
			ui->caps_notice_label->show();
		}
		else {
			ui->caps_notice_label->hide();
		}
	}
	else {
		ui->caps_notice_label->hide();
	}
}

bool UserRegisterWidget::checkUsernameRegular()
{
	QString username = this->username_lineEdit->text();
	if (nullptr == username || username.length() <= 0) {
		return false;
	}
	else {
		if (!usernameRange->exactMatch(username)) {
			ui->username_notice_label->show();
			ui->username_notice_label
				->setText("<html><head/><body><p><span style=\" color:#ff0000;\">用户名不规范</span></p></body></html>");
			return false;
		}
		if (username.length() < 8 || username.length() > 20) {
			ui->username_notice_label->show();
			ui->username_notice_label
				->setText("<html><head/><body><p><span style=\" color:#ff0000;\">用户名长度需要8-20位</span></p></body></html>");
			return false;
		}
		if (!lowercase->exactMatch(username) &&
			!uppercase->exactMatch(username)
			) {
			ui->username_notice_label->show();
			ui->username_notice_label
				->setText("<html><head/><body><p><span style=\" color:#ff0000;\">用户名需包含字母</span></p></body></html>");
			return false;
		}
		else if (!numbercase->exactMatch(username)) {
			ui->username_notice_label->show();
			ui->username_notice_label
				->setText("<html><head/><body><p><span style=\" color:#ff0000;\">用户名需包含数字</span></p></body></html>");
			return false;
		}
		else {
			ui->username_notice_label->hide();
			return true;
		}
	}
}

bool UserRegisterWidget::checkPassword1Regular()
{

	QString password1 = this->password1_lineEdit->text();
	if (nullptr == password1 || password1.length() <= 0) {
		return false;
	}
	else {
		if (!passwordRange->exactMatch(password1)) {
			ui->password1_notice_label->show();
			ui->password1_notice_label
				->setText("<html><head/><body><p><span style=\" color:#ff0000;\">口令不规范</span></p></body></html>");
			return false;
		}
		if (password1.length() < 10 || password1.length() > 20) {
			ui->password1_notice_label->show();
			ui->password1_notice_label
				->setText("<html><head/><body><p><span style=\" color:#ff0000;\">口令长度需要10-20位</span></p></body></html>");
			return false;
		}
		int matchnum = 0;
		if (lowercase->exactMatch(password1)) {
			matchnum++;
		}
		if (uppercase->exactMatch(password1)) {
			matchnum++;
		}
		if (numbercase->exactMatch(password1)) {
			matchnum++;
		}
		if (speccharcase->exactMatch(password1)) {
			matchnum++;
		}

		if (matchnum >= 3) {
			ui->password1_notice_label->hide();
			return true;
		}
		else {
			ui->password1_notice_label->show();
			ui->password1_notice_label->setText(
				"<html><head/><body><p><span style=\" color:#ff0000;\">口令需包含数字、大写字母、小写字母、特殊符号中至少三种</span></p></body></html>");
			return false;
		}

	}
}

bool UserRegisterWidget::checkPassword2Regular()
{
	QString password1 = this->password1_lineEdit->text();
	QString password2 = this->password2_lineEdit->text();
	qDebug() << Q_FUNC_INFO << "password1 is " << password1;
	qDebug() << Q_FUNC_INFO << "password2 is " << password2;
	if (0 == password1.compare(password2)) {
		ui->password2_notice_label->hide();
		return true;
	}
	else {
		ui->password2_notice_label->show();
		return false;
	}
}

void UserRegisterWidget::slot_username_textChanged(QString usernametext)
{
	qDebug() << Q_FUNC_INFO << "stat is " << usernametext;
	bool ret = checkUsernameRegular();
}

void UserRegisterWidget::slot_password1_textChanged(QString)
{

	bool ret = checkPassword1Regular();
}

void UserRegisterWidget::slot_password2_textChanged(QString)
{

	bool ret = checkPassword2Regular();
}

QString UserRegisterWidget::digest(QString input)
{
	if (nullptr == input || input.length() <= 0) {
		return nullptr;
	}
	uint8 digest[33] = "\0";
	char base64digest[46] = "\0";
	QByteArray bytearray = input.toUtf8();
	uint8 *inputbyte = reinterpret_cast <unsigned char *>(bytearray.data());
	sha256(digest, inputbyte, strlen((char *)inputbyte));
	base64enc(base64digest, digest, strlen((char *)digest));
	QString base64digeststr(base64digest);
	return base64digeststr;
}

void UserRegisterWidget::on_register_pushButton_clicked()
{
	QString username = this->username_lineEdit->text();
	QString password1 = this->password1_lineEdit->text();
	QString password2 = this->password2_lineEdit->text();
	QString Role = getSelectedRole();
	if (checkUsernameRegular() && checkPassword1Regular() && checkPassword2Regular()) {
		QString base64digeststr = digest(password1);
		qDebug() << Q_FUNC_INFO << "base64digeststr is " << base64digeststr;

		qDebug() << Q_FUNC_INFO << "role is " << Role;

		userRegister(username, base64digeststr, Role);
	}
}

void UserRegisterWidget::userRegister(QString username, QString password, QString role)
{
	if (nullptr == ur_manager) {
		ur_manager = new QNetworkAccessManager(this);
		connect(ur_manager, SIGNAL(finished(QNetworkReply * )),
				this, SLOT(slot_ur_replyFinished(QNetworkReply * )));
		connect(ur_manager, SIGNAL(sslErrors(QNetworkReply * , QList<QSslError>)),
				this, SLOT(slot_ur_sslErrors(QNetworkReply * , QList<QSslError>)));
		connect(ur_manager, SIGNAL(authenticationRequired(QNetworkReply * , QAuthenticator * )),
				this, SLOT(slot_ur_provideAuthenication(QNetworkReply * , QAuthenticator * )));
	}
	QNetworkRequest ur_request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_2);
	ur_request.setSslConfiguration(config);
	ur_request.setUrl(QUrl(url + "userregister"));
	ur_request.setRawHeader("Content-Type", "application/json");

	QVariantMap messagejsonvar;
	messagejsonvar.insert("method", "userregister");
	messagejsonvar.insert("version", "1.0");
	QVariantMap requestvar;
	requestvar.insert("username", username);
	requestvar.insert("password", password);
	requestvar.insert("role", role);
	messagejsonvar.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	messagejsonvar.insert("request", requestvar);
	QJsonObject obJct = QJsonObject::fromVariantMap(messagejsonvar);
	QJsonDocument jsonDoc(obJct);
	QByteArray json = jsonDoc.toJson();
	QString messagejsonstr(json);


	qDebug() << Q_FUNC_INFO << "messagejsonstr is " << messagejsonstr;

	ur_post_reply = ur_manager->post(ur_request, messagejsonstr.toUtf8());
	connect(ur_post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
			this, SLOT(slot_ur_NetWorkError(QNetworkReply::NetworkError)));
}

void UserRegisterWidget::keyReleaseEvent(QKeyEvent *event)
{
	if (nullptr != event && event->key() == Qt::Key_CapsLock) {
		if (CapsButtonState()) {
			ui->caps_notice_label->show();
		}
		else {
			ui->caps_notice_label->hide();
		}
	}
	else {

	}
}
void UserRegisterWidget::keyPressEvent(QKeyEvent *event)
{

}

void UserRegisterWidget::slot_replyFinished(QNetworkReply *reply)
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
							if (detailsiteraterkey == "ifexist") {
								ifexist = detailsiteratervalue.toBool();
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
		qDebug() << Q_FUNC_INFO << "ifexist is " << ifexist;

		if (ifexist) {
			ui->username_notice_label->show();
			ui->username_notice_label
				->setText("<html><head/><body><p><span style=\" color:#ff0000;\">用户名已存在</span></p></body></html>");
		}
		else {
			ui->username_notice_label->show();
			ui->username_notice_label
				->setText("<html><head/><body><p><span style=\" color:##00aaff;\">用户名不存在，可以注册</span></p></body></html>");

		}
		reply->deleteLater();
		reply = nullptr;
	}
}

void UserRegisterWidget::slot_ur_replyFinished(QNetworkReply *reply)
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
	QString username;
	if (nullptr != reply) {
		ret_data = reply->readAll();
		qDebug() << Q_FUNC_INFO << "reply readAll is " << ret_data;
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
							if (detailsiteraterkey == "username") {
								username = detailsiteratervalue.toString();
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

		if (result.compare("success") == 0 &&
			code.compare("8000") == 0 &&
			method.compare("userregister") == 0 &&
			message.compare("user register success") == 0
			) {
			ui->registerresult_label->show();
			ui->registerresult_label
				->setText("<html><head/><body><p><span style=\" color:#00aaff;\">注册成功</span></p></body></html>");
		}
		else {
			ui->registerresult_label->show();
			ui->registerresult_label->setText(
				"<html><head/><body><p><span style=\" color:#ff0000;\">" + message + "</span></p></body></html>");

		}
		reply->deleteLater();
		reply = nullptr;
	}

}

void UserRegisterWidget::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	QSslCertificate sslcert = errors[0].certificate();
	reply->ignoreSslErrors();
}

void UserRegisterWidget::slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator)
{

}

QString UserRegisterWidget::getSelectedRole() const
{
    return roleComboBox->currentText();
}

void UserRegisterWidget::slot_NetWorkError(QNetworkReply::NetworkError errorCode)
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

void UserRegisterWidget::slot_ur_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	QSslCertificate sslcert = errors[0].certificate();
	reply->ignoreSslErrors();
}

void UserRegisterWidget::slot_ur_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator)
{

}

void UserRegisterWidget::slot_ur_NetWorkError(QNetworkReply::NetworkError errorCode)
{
	if (nullptr != ur_post_reply) {
		ur_post_reply->deleteLater();
		ur_post_reply = nullptr;
	}
	if (nullptr != ur_manager) {
		delete ur_manager;
		ur_manager = nullptr;
	}
}

UserRegisterWidget::~UserRegisterWidget()
{
	delete ui;
}

void UserRegisterWidget::on_close_pushButton_clicked()
{
	this->close();
}
