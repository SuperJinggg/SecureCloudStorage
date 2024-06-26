#include "userentitywidget.h"
#include "global.h"
#include "ui_userentitywidget.h"
#include "securityadminwidget.h"

UserEntityWidget::UserEntityWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserEntityWidget)
{
    ui->setupUi(this);
}


int UserEntityWidget::getid(){
    return this->id;
}
QString UserEntityWidget::getusername(){
    return this->username;
}

QString UserEntityWidget::getrole(){
    return this->role;
}
QString UserEntityWidget::getstate(){
    return this->state;
}
QString UserEntityWidget::getregistertime(){
    return this->registertime;
}
QString UserEntityWidget::getforbid_time(){
    return this->forbid_time;
}
QTableWidgetItem *UserEntityWidget::getqcheckbox(){
    return this->qcheckbox;
}


void UserEntityWidget::setid(int id){
    this->id=id;
}
void UserEntityWidget::setusername(QString username){
    this->username=username;
}
void UserEntityWidget::setrole(QString role){
    this->role=role;
}
void UserEntityWidget::setstate(QString state){
    this->state=state;
}
void UserEntityWidget::setregistertime(QString registertime){
    this->registertime=registertime;
}
void  UserEntityWidget::setforbid_time(QString forbid_time){
    this->forbid_time=forbid_time;
}
void UserEntityWidget::setqcheckbox(QTableWidgetItem * qcheckbox){
    this->qcheckbox=qcheckbox;
}


UserEntityWidget::~UserEntityWidget()
{
    delete ui;
}

void UserEntityWidget::on_operationwidget_delete_pushButton_clicked()
{
    QString username = this->getusername();
    sendrequest(username);
}

void UserEntityWidget::sendrequest(QString username){
    if (nullptr == manager) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(slot_replyFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(slot_provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }
    QNetworkRequest request;
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config);
    request.setUrl(QUrl(url+"userdelete"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("sessionid", sessionid.toUtf8());

    QVariantMap messagejsonvar;
    messagejsonvar.insert("method", "userdelete");
    messagejsonvar.insert("version", "1.0");
    QVariantMap requestvar;
    requestvar.insert("username", username);
    messagejsonvar.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    messagejsonvar.insert("request", requestvar);
    QJsonObject obJct = QJsonObject::fromVariantMap(messagejsonvar);
    QJsonDocument jsonDoc(obJct);
    QByteArray json = jsonDoc.toJson();
    QString messagejsonstr(json);
    qDebug()<<Q_FUNC_INFO<<"messagejsonstr is "<<messagejsonstr;

    post_reply = manager->post(request,messagejsonstr.toUtf8());
    connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}



void UserEntityWidget::slot_replyFinished(QNetworkReply* reply){


    QString ret_data;
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
        qDebug() << Q_FUNC_INFO << "reply readAll is " << ret_data;

        QString method="";
        QString result="";
        QString code = "";
        QString message = "";
        QString role = "";
        QVariantMap details;
        QVariantMap userinfo;
        QString username;
        QString isdelete;


        QByteArray resultjsonbytearray;
        resultjsonbytearray.append(ret_data);
        QJsonParseError parseresult;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(resultjsonbytearray, &parseresult);
        if (parseresult.error == QJsonParseError::NoError)
        {
            if (parse_doucment.isObject()){
                QVariantMap parsemap = parse_doucment.toVariant().toMap();
                QMapIterator<QString, QVariant> iterater(parsemap);
                while (iterater.hasNext()) {
                    iterater.next();
                    QString iteraterkey = iterater.key();
                    QVariant iteratervalue= iterater.value();
                    if(iteraterkey=="method"){
                        method = iteratervalue.toString();
                    }else if(iteraterkey=="result"){
                        isdelete = iteratervalue.toString();
                    }
                    else if(iteraterkey=="code"){
                        code = iteratervalue.toString();
                    }
                    else if(iteraterkey=="message"){
                        message = iteratervalue.toString();
                    }
                    else if (iteraterkey=="details"){
                        details = iteratervalue.toMap();
                        QMapIterator<QString, QVariant> detailsiterater(details);
                        while (detailsiterater.hasNext()) {
                            detailsiterater.next();
                            QString detailsiteraterkey = detailsiterater.key();
                            QVariant detailsiteratervalue= detailsiterater.value();
                            if(detailsiteraterkey=="userinfo"){
                                userinfo = detailsiteratervalue.toMap();
                                QMapIterator<QString, QVariant> userinfoiterater(userinfo);
                                while(userinfoiterater.hasNext()){
                                    userinfoiterater.next();
                                    QString userinfoiteraterkey = userinfoiterater.key();
                                    QVariant userinfoiteratervalue= userinfoiterater.value();
                                    if(userinfoiteraterkey=="role")
                                        role = userinfoiteratervalue.toString();
                                }

                            }
                        }
                    }

                }
                if(isdelete == "success"){
                    emit refresh(role);
                }


            }
        }

    }
}
void UserEntityWidget::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void UserEntityWidget::slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator) {

}

void UserEntityWidget::slot_NetWorkError(QNetworkReply::NetworkError errorCode) {
    if (nullptr != post_reply) {
        post_reply->deleteLater();
        post_reply = nullptr;
    }
    if (nullptr != manager) {
        delete manager;
        manager = nullptr;
    }
}

