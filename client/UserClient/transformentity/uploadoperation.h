#ifndef UPLOADOPERATION_H
#define UPLOADOPERATION_H

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
#include <QJsonParseError>
#include <QListWidgetItem>
#include <QJsonObject>

#include "../lib/include/cipherinterface.h"
#include "../global.h"
#include "../inc/sha256.h"
#include "../inc/aes_cbc.h"
#include "../inc/base64_enc.h"
#include "../transformlistform/uploadlistform.h"



class UploadOperation : public QObject
{
    Q_OBJECT
public:
    explicit UploadOperation(QObject *parent = nullptr);
    explicit UploadOperation(QString parentid,QString plainfile,QString cipherfile,QObject *parent = nullptr);

    void DoSetup(QThread &cThread);
    void encryptfile();
    void generateCipherIndex();
    void generateMetadata();
    QString calcfilehash(QString path, QString alg_method);
    QString calcstrhash(QString input);
    char* getTextStr(QString string);
    void uploaddata();
    void aes_cbc_pkcs5_encrypt(char* key , int keylen , char* pcInput, int nLen, char* pcOut);


    void uploadotherdata(QString,QString,QString,QString,QString);

signals:
    void setstat(QString);
    void toSetProgressBar(int);

public slots:

    void slot_data_opeartion();
    void slot_qprogress_finished(int, QProcess::ExitStatus);
    void slot_qprogress_readyReadStandardOutput();
    void slot_qprogress_started();

    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);
    void progressChanged(qint64 progress, qint64 total);



    void slot_otherdata_replyFinished(QNetworkReply* reply);
    void slot_otherdata_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_otherdata_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_otherdata_NetWorkError(QNetworkReply::NetworkError);

private:
    QString parentid;
    QString plainfile;
    QString cipherfile;
    QString file_unique_id;

    QString encryptkey="";
    QString doc_plaintext;
    QString encrypted_index = "";
    QString metadata = "";
    QString encrypt_retqstr = "";


//    JsonObject metadatajsonobj;//初始化json对象

    QNetworkAccessManager *manager{ manager = nullptr };
    QNetworkReply *post_reply{ post_reply = nullptr };
    /*	缓冲区大小*/
    #define BUFFER_SIZE 1024


    QNetworkAccessManager *otherdatamanager{ otherdatamanager = nullptr };
    QNetworkReply *otherdatapost_reply{ otherdatapost_reply = nullptr };




};

#endif // UPLOADOPERATION_H
