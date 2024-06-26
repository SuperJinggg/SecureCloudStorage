#ifndef SYSTEMADMINWIDGET_H
#define SYSTEMADMINWIDGET_H

#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QStandardItemModel>
#include "base64_enc.h"


namespace Ui
{
class SystemAdminWidget;
}

class SystemAdminWidget: public QWidget
{
Q_OBJECT

public:
	explicit SystemAdminWidget(QWidget *parent = 0);
	~SystemAdminWidget();

private slots:
	void on_system_management_pushButton_clicked();

	void slot_replyFinished(QNetworkReply *reply);
	void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
	void slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator);
	void slot_NetWorkError(QNetworkReply::NetworkError);

	void system_audit();


private:
	Ui::SystemAdminWidget *ui;

	QNetworkAccessManager *manager{manager = nullptr};
	QNetworkReply *post_reply{post_reply = nullptr};

	QString username = "systemadmin";
	QString password = "09bd600e-1094-4264-a885-204b7c0e1674";

	QString init_endpoints[5];
	QString endpoints[2];

	bool init = true;
	QVariant maxmem;

	QString dataConvert(long long data);
};

#endif // SYSTEMADMINWIDGET_H
