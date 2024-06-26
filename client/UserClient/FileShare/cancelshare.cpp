#include "cancelshare.h"

CancelShare::CancelShare(QObject *parent) : QObject(parent)
{

}
void CancelShare::cancel_share(QString fileuniqueid , QString sharedusername){
    if (nullptr == net_manager) {
        net_manager = new QNetworkAccessManager(this);
        connect(net_manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(slot_replyFinished(QNetworkReply*)));
        connect(net_manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(net_manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(slot_provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }
    QNetworkRequest network_request;
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_2);
    network_request.setSslConfiguration(config);
    network_request.setUrl(QUrl(url+"cancelshare"));
    network_request.setRawHeader("sessionid", sessionid.toUtf8());
    network_request.setRawHeader("Content-Type", "application/json");
    //    QVariantList varList;
    QVariantMap var;
    var.insert("method", "cancelshare");
    var.insert("version", "1.0");
    var.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    QVariantMap requestvar;
    requestvar.insert("fileuniqueid", fileuniqueid);
    requestvar.insert("sharedusername", sharedusername);
    var.insert("request",requestvar);
    QJsonObject obJct = QJsonObject::fromVariantMap(var);
    QJsonDocument jsonDoc(obJct);
    QByteArray json = jsonDoc.toJson();
    QString messagejsonstr(json);
    //    messagejsonstr = QtJson::serialize(messagejsonobj);
    qDebug()<<Q_FUNC_INFO<<"messagejsonstr is "<<messagejsonstr;
    post_reply = net_manager->post(network_request,messagejsonstr.toUtf8());
    connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}
void CancelShare::slot_replyFinished(QNetworkReply* reply){
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
        //        qDebug() <<Q_FUNC_INFO<< "ret_data =" << ret_data;
        QString retmethod;
        QString retresult;
        QString retmessage;
        int retcode;
        QString retrole;
        QString retusername;
        QString retsessionid;
        QString retsessionkey;
        QString retlastauthtime;
        QString retlastauthaddress;

        //解析json
        QByteArray jsonbytearray;
        jsonbytearray.append(ret_data);

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonbytearray, &json_error));

        if(json_error.error != QJsonParseError::NoError)
        {
            qDebug()<< Q_FUNC_INFO << "json error!";
            return;
        }
        QJsonObject rootObj = jsonDoc.object();
        if(rootObj.contains("method"))
        {
            retmethod = rootObj.value("method").toString();
        }
        emit signal_cancel_result("cancel success");
    }
}
void CancelShare::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}
void CancelShare::slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}
void CancelShare::slot_NetWorkError(QNetworkReply::NetworkError){
    if (nullptr != post_reply) {
        post_reply->deleteLater();
        post_reply = nullptr;
    }
    if (nullptr != net_manager) {
        delete net_manager;
        net_manager = nullptr;
    }
}
