#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QJsonParseError>

#include "sha256.h"
#include "base64_enc.h"
#include "hmac_sha256.h"
#include "local_lineedit.h"
#include "global.h"
#include "./LocalLog/locallogdialog.h"
#include "./LocalLog/seccloudlog.h"


namespace Ui
{
class LoginWidget;
}

class LoginWidget: public QWidget
{
Q_OBJECT

public:
	friend class TestLoginGui;
	explicit LoginWidget(QWidget *parent = nullptr);
	~LoginWidget();


private slots:
	void slot_replyFinished(QNetworkReply *reply);
	void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
	void slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator);
	void slot_NetWorkError(QNetworkReply::NetworkError);

	void slot_password_linedit_focussed(bool);//口令1输入框获取或失去鼠标焦点出发的槽函数
	void on_pushButton_Login_clicked();
	void on_pushButton_clicked();
	void on_authcode_label_clicked();

private:
	Ui::LoginWidget *ui;
	Local_LineEdit *lineEdit_password{lineEdit_password = nullptr};
	QString getSelectedRole() const;
	QString saltvalue = "e139f8baPdec0X478cV9830Hcbb8f6a5";
	void refreshcode();
	QComboBox *comboBox;
	QString digest(QString input);

	void userlogin(QString, QString, QString, QString);
	QNetworkAccessManager *login_manager{login_manager = nullptr};
	QNetworkReply *login_post_reply{login_post_reply = nullptr};

	QString random = "";
};

#endif // LOGINWIDGET_H
