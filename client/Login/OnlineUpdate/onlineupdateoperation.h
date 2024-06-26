#ifndef ONLINEUPDATEOPERATION_H
#define ONLINEUPDATEOPERATION_H

#include <QObject>
#include "../LocalLog/seccloudlog.h"
#include <QThread>
#include <QFile>
#include <QVariant>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QApplication>
#include <QDir>
#include <QProcess>

class OnlineUpdateOperation : public QObject
{
    Q_OBJECT
public:
    explicit OnlineUpdateOperation(QObject *parent = nullptr);
    void DoSetup(QThread &cThread);
    void toInstall(QString exePath);
signals:

    void toSetProgressBar(int);
public slots:

    void slot_opeartion();

    void httpReadyRead();
    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);
    void progressChanged(qint64 progress, qint64 total);

private:
    QNetworkAccessManager *manager{ manager = nullptr };
    QNetworkReply *reply{ reply = nullptr };
    QFile *file {file=nullptr};

    void gotoclear();
    QString exepath;
};

#endif // ONLINEUPDATEOPERATION_H
