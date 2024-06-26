#ifndef ONLINEUPDATEDIALOG_H
#define ONLINEUPDATEDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QtGui>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>

#include "../global.h"
#include "../LocalLog/seccloudlog.h"
#include "./onlineupdateoperation.h"


namespace Ui {
class OnlineUpdateDialog;
}

class OnlineUpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OnlineUpdateDialog(QWidget *parent = nullptr);
    ~OnlineUpdateDialog();
    void get_version();

private slots:
    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);


    void on_cancel_btn_clicked();

    void on_update_btn_clicked();


    void slot_setprogressbar(int);

signals:
    void signal_updateopeartion_begin();

private:
    Ui::OnlineUpdateDialog *ui;
    QNetworkAccessManager* manager{manager=nullptr};
    QNetworkReply *post_reply{ post_reply = nullptr };

    QThread *updatethread{updatethread=nullptr};
    OnlineUpdateOperation * onlineupdateoperation {onlineupdateoperation = nullptr};

};

#endif // ONLINEUPDATEDIALOG_H
