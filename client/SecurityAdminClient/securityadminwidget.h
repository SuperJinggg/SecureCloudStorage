#ifndef SECURITYADMINWIDGET_H
#define SECURITYADMINWIDGET_H

#include <QWidget>
#include "userentitywidget.h"
#include "userregisterwidget.h"

namespace Ui {
class SecurityAdminWidget;
}

class SecurityAdminWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SecurityAdminWidget(QWidget *parent = 0);
    ~SecurityAdminWidget();
    void securityadmin_userlist(int index);


public slots:
    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);
    void sendrequest(QString role);

private slots:
    void on_pushButton_usermanagement_clicked();

    void on_pushButton_operation_audit_clicked();


    void refresh_view(QTableWidget *qtablewidget);

    void on_add_securityadmin_pushButton_clicked();

    void on_auditadmin_tabWidget_currentChanged(int index);

    void on_delete_usermanager_pushButton_clicked();

private:
    Ui::SecurityAdminWidget *ui;
    QMap<QString , UserEntityWidget*> *userentity_list{userentity_list=nullptr};
    UserRegisterWidget w;

    QNetworkAccessManager *manager{ manager = nullptr };
    QNetworkReply *post_reply{ post_reply = nullptr };


    int usernumber=0;
};

#endif // SECURITYADMINWIDGET_H
