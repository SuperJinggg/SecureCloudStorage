#ifndef CANCELSHARE_H
#define CANCELSHARE_H

#include <QObject>
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
#include "./datashareentity.h"
#include "global.h"
#include "../entities/filemetadata.h"

class CancelShare : public QObject
{
    Q_OBJECT
public:
    explicit CancelShare(QObject *parent = nullptr);

    void cancel_share(QString fileuniqueid ,QString sharedusername);


signals:
    void signal_cancel_result(QString);

public slots:
    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);
private:

    QNetworkAccessManager *net_manager{ net_manager = nullptr };
    QNetworkReply *post_reply{ post_reply = nullptr };
};

#endif // CANCELSHARE_H
