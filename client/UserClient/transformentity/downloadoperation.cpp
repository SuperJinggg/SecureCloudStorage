#include "downloadoperation.h"

DownloadOperation::DownloadOperation(QObject *parent) : QObject(parent)
{

}

DownloadOperation::DownloadOperation(
        QString savepath,
        FileMetadata* downloadfilemetadata,
        QObject *parent) : QObject(parent)
{
    this->savepath=savepath;
    this->downloadfilemetadata=downloadfilemetadata;
}


void DownloadOperation::DoSetup(QThread &cThread) {
    connect(&cThread, SIGNAL(started()), this, SLOT(slot_data_opeartion()));
}

void DownloadOperation::slot_data_opeartion(){


    if (!QDir(file_path).exists())
    {
        QDir photoDir;
        photoDir.mkdir(file_path);
    }

    QString filename = this->downloadfilemetadata->getfilename();

    this->downloadfilemetadata->setsavepath(file_path + "/" + filename);

    file = new QFile(file_path + "/" + filename);

    if(file->exists()){
        qDebug()<<Q_FUNC_INFO<<"FILE is "<<file_path + "/" + filename;
        file->remove();
    }

    if (!file->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        //如果打开文件失败，则删除file，并使file指针为0，然后返回
        //qDebug() << "file open error";
        delete file;
        file = nullptr;
        return;
    }


    if (nullptr == manager) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(slot_replyFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(slot_provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }

    QUrl downloaddataurl(url+"datadownload");
    QNetworkRequest request(downloaddataurl);
    request.setRawHeader("sessionid", sessionid.toUtf8());
    request.setRawHeader("Content-Type", "application/json");
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config);
    QVariantMap var;
    var.insert("method", "datadownload");
    var.insert("version", "1.0");
    QDateTime currenttime = QDateTime::currentDateTime();
    var.insert("timestamp", currenttime.toString("yyyy-MM-dd HH:mm:ss"));
    QVariantMap requestvar;
    requestvar.insert("fileuniqueid", this->downloadfilemetadata->getfileuniqueid());
    var.insert("request",requestvar);
    QJsonObject obJct = QJsonObject::fromVariantMap(var);
    QJsonDocument jsonDoc(obJct);
    QByteArray json = jsonDoc.toJson();
    QString messagejsonstr(json);
    qDebug()<<Q_FUNC_INFO<<"messagejsonstr is "<<messagejsonstr;

    post_reply = manager->post(request,messagejsonstr.toUtf8());
    connect(post_reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
    connect(post_reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(progressChanged(qint64, qint64)));
    connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));

    //    emit toSetProgressBar(100);
}
void DownloadOperation::httpReadyRead() {
    qDebug()<<Q_FUNC_INFO<<"in httpReadyRead";
    if (file && post_reply->bytesAvailable() > 50000)
    {
        QByteArray bytes = post_reply->read(post_reply->bytesAvailable());
        file->write(bytes);
        file->flush();
    }
}

void DownloadOperation::slot_replyFinished(QNetworkReply* reply){

    qDebug()<<Q_FUNC_INFO<<"in slot_replyFinished";
    gotoclear();
    QList<QNetworkReply::RawHeaderPair> ret_message_header = reply->rawHeaderPairs();
    if (nullptr != post_reply) {
        //ret_data = post_reply->readAll();
        //qDebug() << Q_FUNC_INFO << "reply readAll is " << ret_data;
        post_reply->deleteLater();
        post_reply = nullptr;
    }
}

void DownloadOperation::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void DownloadOperation::slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}

void DownloadOperation::slot_NetWorkError(QNetworkReply::NetworkError){
    if (nullptr != post_reply) {
        post_reply->deleteLater();
        post_reply = nullptr;
    }
    if (nullptr != manager) {
        delete manager;
        manager = nullptr;
    }
}

void DownloadOperation::progressChanged(qint64 progress, qint64 total){
    int pro = 0;
    if (0 != total) {
        pro = (int)(progress * 100 / total);
        qDebug() << Q_FUNC_INFO << "pro is " << pro;
        emit toSetProgressBar(pro);
    }
}
void DownloadOperation::gotoclear() {
    if (file && post_reply->bytesAvailable() > 0) {
        QByteArray bytes = post_reply->read(post_reply->bytesAvailable());
        file->write(bytes);
        file->flush();
    }
    if (nullptr != file) {
        file->flush();
        file->close();
        file = 0;
        delete file;
    }
}


void DownloadOperation::slot_decrypt_and_open_file(){
    //    QString cipher_path = filemetadataentity->get_store_path();
    //    QString from_file_path = cipher_file_path +"/"+ cipher_path;

    //    QString qapplicationpath = QCoreApplication::applicationDirPath();
    //    QString plainfilepath =qapplicationpath+"/decrypt/"+cipher_path;
    //    QFileInfo fileinfo(plainfilepath);
    //    string decryptret = YunLock_DecryptFile(from_file_path.toStdString(),
    //                                                plainfilepath.toStdString(),
    //                                                cipher_key.toStdString());
    //        QString decrypt_ret_str = QString::fromStdString(decryptret);

    qDebug()<<Q_FUNC_INFO<<"slot_decrypt_and_open_file";
    //发送请求获取解密密钥
    if (nullptr == getkey_manager) {
        getkey_manager = new QNetworkAccessManager(this);
        connect(getkey_manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(slot_getkey_replyFinished(QNetworkReply*)));
        connect(getkey_manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_getkey_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(getkey_manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(slot_getkey_provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }
    QNetworkRequest network_request;
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_2);
    network_request.setSslConfiguration(config);
    network_request.setUrl(QUrl(url+"getkey"));

    network_request.setRawHeader("sessionid", sessionid.toUtf8());
    network_request.setRawHeader("Content-Type", "application/json");
    QVariantMap messagejsonvar;
    messagejsonvar.insert("method", "getkey");
    messagejsonvar.insert("version", "1.0");
    messagejsonvar.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    QVariantMap requestvar;
    requestvar.insert("fileuniqueid", downloadfilemetadata->getfileuniqueid());
    messagejsonvar.insert("request", requestvar);
    QJsonObject obJct = QJsonObject::fromVariantMap(messagejsonvar);
    QJsonDocument jsonDoc(obJct);
    QByteArray json = jsonDoc.toJson();
    QString messagejsonstr(json);
    //    qDebug()<<Q_FUNC_INFO<<"messagejsonstr is "<<messagejsonstr;
    getkey_post_reply = getkey_manager->post(network_request,messagejsonstr.toUtf8());
    connect(getkey_post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slot_getkey_NetWorkError(QNetworkReply::NetworkError)));

}


void DownloadOperation::slot_getkey_replyFinished(QNetworkReply* reply){
    QByteArray ret_data;
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() <<Q_FUNC_INFO<< "status code=" << statusCode.toInt();
    QString method="";
    QString result="";
    QString code = "";

    QString message = "";
    QVariantMap details;
    bool ifexist;
    if (nullptr != reply) {
        ret_data = reply->readAll();
        qDebug() <<Q_FUNC_INFO<< "ret_data is " << ret_data;
        QString retmethod;
        QString retresult;
        QString retmessage;
        QString retcode;
        QString rettimestamp;
        QVariantMap details;
        QString fileencryptkey;
        QString fileuniqueid;
        QString savetime;
        //解析json
        QByteArray jsonbytearray;
        jsonbytearray.append(ret_data);
        QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(jsonbytearray, &json_error);
        if (json_error.error == QJsonParseError::NoError)
        {

            if (parse_doucment.isObject()){
                QVariantMap parsemap = parse_doucment.toVariant().toMap();
                QMapIterator<QString, QVariant> iterater(parsemap);
                while (iterater.hasNext()) {
                    iterater.next();
                    QString iteraterkey = iterater.key();
                    QVariant iteratervalue= iterater.value();
                    if(iteraterkey=="method"){
                        retmethod = iteratervalue.toString();
                    }else if(iteraterkey=="result"){
                        retresult = iteratervalue.toString();
                    }
                    else if(iteraterkey=="code"){
                        retcode = iteratervalue.toString();
                    }
                    else if(iteraterkey=="message"){
                        retmessage = iteratervalue.toString();
                    }
                    else if(iteraterkey=="details"){
                        details = iteratervalue.toMap();
                        QMapIterator<QString, QVariant> detailsiterater(details);
                        while (detailsiterater.hasNext()) {
                            detailsiterater.next();
                            QString detailsiteraterkey = detailsiterater.key();
                            QVariant detailsiteratervalue= detailsiterater.value();
                            if(detailsiteraterkey=="fileencryptkey"){
                                fileencryptkey = detailsiteratervalue.toString();
                            }else if(detailsiteraterkey=="fileuniqueid"){
                                fileuniqueid = detailsiteratervalue.toString();
                            }
                            else if(detailsiteraterkey=="savetime"){
                                savetime = detailsiteratervalue.toString();
                            }
                        }
                    }
                }
            }
        }else{
            qDebug() << Q_FUNC_INFO << "解析json失败";
        }

        QByteArray sessionkeyarray =  sessionkey.toUtf8();
        char* key = sessionkeyarray.data();
        //        char* input = "eMMs0p6EtURtAcfsUkPVXmZf7UT2jRJhyuFlhghK2+LM7waHRbkWuXlIHUQse8zuAzdmFkJGc/xxEU0gbyFYkMKcvgY7NRwe8oqayh0vNnc=";
        QByteArray encryptkeybytearray = fileencryptkey.toUtf8();
        char* input =encryptkeybytearray.data();
        char output [83]="\0";
        aes_cbc_pkcs5_decrypt(key,strlen(key),input,encryptkeybytearray.length(),output);

        qDebug()<<Q_FUNC_INFO<<"解密后的密钥 is "<<output;



        QString cipher_path = this->downloadfilemetadata->getfilename();
        QString from_file_path = this->downloadfilemetadata->getsavepath();

        QString decrept_key(output); //32位解密密钥

        QString currentrunpath = QCoreApplication::applicationDirPath();

        QString decreptpath = currentrunpath+"/decrypt";

        if (!QDir(decreptpath).exists())
        {
            QDir decreptDir;
            decreptDir.mkdir(decreptpath);
        }

        QString filename = this->downloadfilemetadata->getfilename();


        QString decryptpath = decreptpath+"/"+filename;//解密后的明文路径


        qDebug()<<Q_FUNC_INFO<<"密文文件路径："<<from_file_path;

        qDebug()<<Q_FUNC_INFO<<"解密后明文的路径："<<decryptpath;

        qDebug()<<Q_FUNC_INFO<<"解密密钥： "<<decrept_key;


        string decrypt_ret =  YunLock_DecryptFile
                (from_file_path.toStdString(),
                 decryptpath.toStdString(),
                 decrept_key.toStdString());
        QString decrypt_retqstr = QString::fromStdString(decrypt_ret);
        qDebug()<<"decrypt_retqstr ="<< decrypt_retqstr;
        qDebug()<<"解密成功";
        OpenFile(decryptpath);
    }
}

void DownloadOperation::slot_getkey_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}
void DownloadOperation::slot_getkey_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}
void DownloadOperation::slot_getkey_NetWorkError(QNetworkReply::NetworkError){
    if (nullptr != getkey_post_reply) {
        getkey_post_reply->deleteLater();
        getkey_post_reply = nullptr;
    }
    if (nullptr != getkey_manager) {
        delete getkey_manager;
        getkey_manager = nullptr;
    }
}



void DownloadOperation::OpenFile(QString filename) {//打开文件操作，filename为明文文件路径
#ifdef Q_OS_WIN //如果是Windows操作系统
    QString file = filename;
    HINSTANCE  nRes = (ShellExecuteW(NULL
                                     , QString("open").toStdWString().c_str()
                                     , file.toStdWString().c_str()
                                     , NULL
                                     , NULL
                                     , SW_SHOW
                                     ));
    int nRes2 = 0;
    char* cmd = QString("rundll32 shell32, OpenAs_RunDLL %1").arg(file).toLocal8Bit().data();
    if ((qint64)nRes <= SE_ERR_NOASSOC) { //ShellExecuteW 返回值小于32表示执行出现错误
        nRes2 = WinExec(cmd, SW_SHOWNORMAL); //调用打开方式对话框
    }
#endif
}


void DownloadOperation::aes_cbc_pkcs5_decrypt(char* key , int keylen , char* input, int inputlen, char* pcOut)
{
    char iv[17] = "\0";
    AES_KEY AES;
    int base64declength = base64_binlength(input,1);
    //    qDebug()<<Q_FUNC_INFO<<"base64declength is "<<base64declength;
    char *dest = (char*)malloc(base64declength + 1);
    base64dec(dest,input,1);
    //    char *out = (char*)malloc(base64declength+1);
    if (AES_set_decrypt_key((unsigned char*)key, keylen*8, &AES) < 0)
    {
        fprintf(stderr, "Unable to set encryption key in AES\n");
        exit(-1);
    }
    AES_cbc_encrypt((unsigned char *)dest,
                    (unsigned char*)pcOut,
                    base64declength,
                    &AES,
                    (unsigned char*)iv,
                    AES_DECRYPT);
    pcOut[64] = 0;
    //    qDebug()<<Q_FUNC_INFO<<"pcOut is "<<QString(pcOut);
    if(nullptr!=dest){
        free(dest);
    }
}
