#include "onlineupdateoperation.h"

OnlineUpdateOperation::OnlineUpdateOperation(QObject *parent) : QObject(parent)
{

}
void OnlineUpdateOperation::DoSetup(QThread &cThread) {
    connect(&cThread, SIGNAL(started()), this, SLOT(slot_opeartion()));
}



void OnlineUpdateOperation::slot_opeartion(){
#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_
    QString logmessage;
    QTextStream logmessageout(&logmessage);
    logmessageout << "in slot_opeartion";
    SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage,1);
#endif
    QDir dir;
    QString configPath = dir.currentPath();
    QString savepath = configPath+"/update";
    if (!QDir(savepath).exists())
    {
        QDir saveDir;
        saveDir.mkdir(savepath);
    }
    exepath = savepath + "/" + "update.exe";
    file = new QFile(exepath);
    if(file->exists()){
        if(file->remove())
        {
            qDebug() << "删除成功";
        }
        else
        {
            qDebug() << "删除失败";
        }
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

    QUrl downloaddataurl(url+"client/SecurityCloudStorageClient.exe");
    QNetworkRequest request(downloaddataurl);
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config);


    reply = manager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(progressChanged(qint64, qint64)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}

void OnlineUpdateOperation::httpReadyRead(){
    //#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_
    //    QString logmessage;
    //    QTextStream logmessageout(&logmessage);
    //    logmessageout << "in httpReadyRead";
    //    SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage,1);
    //#endif


    if (file && reply->bytesAvailable() > 50000)
    {
        QByteArray bytes = reply->read(reply->bytesAvailable());
        file->write(bytes);
        file->flush();
    }


}
void OnlineUpdateOperation::slot_replyFinished(QNetworkReply* reply){
#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_
    QString logmessage;
    QTextStream logmessageout(&logmessage);
    logmessageout << "in slot_replyFinished";
    SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage,1);
#endif
    gotoclear();
    toInstall(exepath);
}
void OnlineUpdateOperation::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}
void OnlineUpdateOperation::slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}
void OnlineUpdateOperation::slot_NetWorkError(QNetworkReply::NetworkError){
    if (nullptr != reply) {
        reply->deleteLater();
        reply = nullptr;
    }
    if (nullptr != manager) {
        delete manager;
        manager = nullptr;
    }
}
void OnlineUpdateOperation::progressChanged(qint64 progress, qint64 total){
    int pro = 0;
    if (0 != total) {
        pro = (int)(progress * 100 / total);
        emit toSetProgressBar(pro);
    }
}

void OnlineUpdateOperation::gotoclear() {
    if (file && reply->bytesAvailable() > 0) {
        QByteArray bytes = reply->read(reply->bytesAvailable());
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
void OnlineUpdateOperation::toInstall(QString exePath)//管理员端只能用第一种方式运行,试试看可行不吧  程序还在运行中  可能没法写入
{
    qDebug()<<Q_FUNC_INFO<<"exePath = "<<exePath;
    QProcess * _mprogress = new QProcess();
    _mprogress->start(exePath);
    QApplication::exit();
}
