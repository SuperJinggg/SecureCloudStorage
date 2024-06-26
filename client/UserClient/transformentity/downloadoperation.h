#ifndef DOWNLOADOPERATION_H
#define DOWNLOADOPERATION_H

#include <QObject>
#include <QProcess>
#include <QDir>
#include <QUuid>
#include <QDebug>
#include <QDateTime>
#include <QHttpMultiPart>
#include <QList>
#include <QFile>
#include <QVariant>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QThread>
#include <QListWidgetItem>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>


#include "../lib/include/cipherinterface.h"
#include "global.h"
#include "sha256.h"
#include "aes_cbc.h"
#include "base64_dec.h"
#include "base64_enc.h"
#include "../transformlistform/uploadlistform.h"

#include <shellapi.h>

class DownloadOperation : public QObject
{
    Q_OBJECT
public:
    explicit DownloadOperation(QObject *parent = nullptr);
    explicit DownloadOperation(QString savepath,FileMetadata* downloadfilemetadata,QObject *parent = nullptr);


    void DoSetup(QThread &cThread);

    void gotoclear();

    void aes_cbc_pkcs5_decrypt(char* key , int keylen , char* input, int inputlen, char* pcOut);

signals:
    void setstat(QString);
    void toSetProgressBar(int);

public slots:
    void slot_data_opeartion();

    void httpReadyRead();
    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);
    void progressChanged(qint64 progress, qint64 total);

    void slot_decrypt_and_open_file();


    void slot_getkey_replyFinished(QNetworkReply* reply);
    void slot_getkey_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_getkey_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_getkey_NetWorkError(QNetworkReply::NetworkError);


private:
    QString savepath;
    QFile *file {file=nullptr};
    FileMetadata* downloadfilemetadata{downloadfilemetadata = nullptr};
    QNetworkAccessManager *manager{ manager = nullptr };
    QNetworkReply *post_reply{ post_reply = nullptr };

    QNetworkReply *getkey_post_reply{ getkey_post_reply = nullptr };
    QNetworkAccessManager *getkey_manager{ getkey_manager = nullptr };

    void OpenFile(QString filename);

};

#endif // DOWNLOADOPERATION_H
