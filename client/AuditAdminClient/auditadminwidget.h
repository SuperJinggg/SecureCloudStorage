#ifndef AUDITADMINWIDGET_H
#define AUDITADMINWIDGET_H

#include <QWidget>
#include <QDateTime>
#include "local_lineedit.h"
#include <QDebug>
#include <QProcess>
#include <QKeyEvent>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include "local_lineedit.h"
#include "sha256.h"
#include "base64_enc.h"
#include "auditentity.h"

namespace Ui
{
class AuditAdminWidget;
}

class AuditAdminWidget: public QWidget
{
Q_OBJECT

public:
	explicit AuditAdminWidget(QWidget *parent = nullptr);
	~AuditAdminWidget();

public slots:
	void slot_audit_jumptopage_linedit_focussed(bool);

	void slot_replyFinished(QNetworkReply *reply);
	void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
	void slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator);
	void slot_NetWorkError(QNetworkReply::NetworkError);


private slots:
	void on_tabWidget_currentChanged(int index);

private:
	Ui::AuditAdminWidget *ui;
	Local_LineEdit *jumptopage_linedit{jumptopage_linedit = nullptr};

	QDateTime audit_starttime;
	QDateTime audit_endtime;
	QString search_str = "";
	QString module = "";
	int show_list_num = 20; //每页最多显示行数
	int current_page = 0; //当前页
	int pagenumber = 0; //总页数
	bool jumptopage_lineedit_focus = false; //页数跳转框是否被鼠标focus

	int totalauditlognumber = 0;

	QNetworkAccessManager *manager{manager = nullptr};
	QNetworkReply *post_reply{post_reply = nullptr};
	void sendrequest(int index);
	void refreshview(int index);
	QMap<QString, AuditEntity *> *auditlog{auditlog = nullptr};
};

#endif // AUDITADMINWIDGET_H
