#include "getsharelist.h"

GetShareList::GetShareList(QObject *parent) : QObject(parent)
{

}

GetShareList::GetShareList(QTableWidget * tablewidget,QObject *parent) : QObject(parent)
{
    this->tablewidget=tablewidget;
//    if(nullptr==sharelist){
//       sharelist = new QList<DataShareEntity*>();
//    }
}

void GetShareList::get_data(){
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
    network_request.setUrl(QUrl(url+"getsharelist"));
    network_request.setRawHeader("sessionid", sessionid.toUtf8());
    network_request.setRawHeader("Content-Type", "application/json");
    //    QVariantList varList;
    QVariantMap var;
    var.insert("method", "getsharelist");
    var.insert("version", "1.0");
    var.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

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

void GetShareList::slot_replyFinished(QNetworkReply* reply){
    QByteArray ret_data;
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid()){

    }
    QString method="";
    QString result="";
    QString code = "";

    QString message = "";
    QVariantMap details;
    bool ifexist;
    if (nullptr != reply) {
        ret_data = reply->readAll();
        qDebug() <<Q_FUNC_INFO<< "ret_data =" << ret_data;
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
        if(rootObj.contains("result"))
        {
            retresult = rootObj.value("result").toString();
        }
        if(rootObj.contains("code"))
        {
            retcode = rootObj.value("code").toInt();
        }
        if(rootObj.contains("message"))
        {
            retmessage = rootObj.value("message").toString();
        }

        QJsonObject detailsobj;
        if(rootObj.contains("details"))
        {
            detailsobj = rootObj.value("details").toObject();
        }
        int filetotalnum;
        if(detailsobj.contains("filenumber")){
            filetotalnum =detailsobj.value("filenumber").toInt();
        }
        QJsonArray filelistarray;

        if(detailsobj.contains("filelist")){
            filelistarray = detailsobj.value("filelist").toArray();
            QList<DataShareEntity*> * sharelist = parse_json_array(filelistarray);
            if(nullptr!=sharelist&&sharelist->size()>0){
                refreshsharelistview(sharelist);
            }
        }
    }
}


void GetShareList::refreshsharelistview(QList<DataShareEntity*> * sharelist){
    qDebug()<<Q_FUNC_INFO<<"in refreshdownloadlistview";
    if (nullptr != sharelist&&sharelist->size() > 0) {
        this->tablewidget->setRowCount(0);
        this->tablewidget->setRowCount(sharelist->size());
        int addrow=0;
        QListIterator<DataShareEntity*> iterater(*sharelist);
        while (iterater.hasNext()) {
            DataShareEntity* entity =  iterater.next();
            QTableWidgetItem *filename_item = new QTableWidgetItem();
            filename_item->setText(entity->getfilename());
            this->tablewidget->setItem(addrow,0,filename_item);
            QTableWidgetItem *filesize_item = new QTableWidgetItem();
            filesize_item->setText(convert_size(entity->getfilesize()));
            this->tablewidget->setItem(addrow,1,filesize_item);
            QTableWidgetItem *towhom_item = new QTableWidgetItem();
            towhom_item->setText(entity->getsharedusername());
            this->tablewidget->setItem(addrow,2,towhom_item);
            QTableWidgetItem *sharetime_item = new QTableWidgetItem();
            sharetime_item->setText(entity->getsharetime().toString("yyyy-MM-dd HH:mm:ss"));
            this->tablewidget->setItem(addrow,3,sharetime_item);
            QPushButton *pushbutton = new QPushButton("取消分享");
            connect(pushbutton,SIGNAL(clicked()),entity,SLOT(cancel_btn_clicked()));
            connect(entity,SIGNAL(signal_cancel_result(QString)),
                    this,SLOT(slot_cancel_result(QString)));

            entity->setpushbutton(pushbutton);
            this->tablewidget->setCellWidget(addrow,4,entity->getpushbutton());
            addrow++;
        }
    }
}

void GetShareList::slot_cancel_result(QString result){
    qDebug()<<Q_FUNC_INFO<<"result is "<<result;
    get_data();
}

QList<DataShareEntity*> * GetShareList::parse_json_array(QJsonArray &filelistarray ){

    QList<DataShareEntity*> *datasharelist = new QList<DataShareEntity*>();
    for(int i = 0; i< filelistarray.size(); i++)
    {
        DataShareEntity* shareentity = new DataShareEntity();
        QJsonObject pobj =  filelistarray.at(i).toObject();
        QString sharedusername;
        if(pobj.contains("sharedusername"))
        {
            sharedusername = pobj.value("sharedusername").toString();
            shareentity->setsharedusername(sharedusername);
        }
        QString filename;
        if(pobj.contains("filename"))
        {
            filename = pobj.value("filename").toString();
            shareentity->setfilename(filename);
        }
        int filesize;
        if(pobj.contains("filesize"))
        {
            filesize = pobj.value("filesize").toInt();
            shareentity->setfilesize(filesize);
        }
        QString fileuniqueid;
        if(pobj.contains("fileuniqueid"))
        {
            fileuniqueid = pobj.value("fileuniqueid").toString();
            shareentity->setfileuniqueid(fileuniqueid);
        }

        if(pobj.contains("sharetime"))
        {
            QString sharetimestr = pobj.value("sharetime").toString();
            QDateTime sharetime;
            sharetime = QDateTime::fromString(sharetimestr, "yyyy-MM-dd HH:mm:ss");
            shareentity->setsharetime(sharetime);
        }
        shareentity->setmark(true);
        datasharelist->append(shareentity);
    }
    return datasharelist;
}
void GetShareList::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}
void GetShareList::slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}
void GetShareList::slot_NetWorkError(QNetworkReply::NetworkError){
    if (nullptr != post_reply) {
        post_reply->deleteLater();
        post_reply = nullptr;
    }
    if (nullptr != net_manager) {
        delete net_manager;
        net_manager = nullptr;
    }
}
QString GetShareList::convert_size(qint64 size){
    QString prefix;
    double speed = size;
    if(speed<=900){
        prefix = "B";
    }

    if(speed>900){
        speed = speed/1024;
        prefix = "K";
    }
    if(speed>900){
        speed = speed/1024;
        prefix = "M";
    }
    if(speed>900){
        speed = speed/1024;
        prefix = "G";
    }
    if(speed>900){
        speed = speed/1024;
        prefix = "T";
    }
    if(speed>900){
        speed = speed/1024;
        prefix = "P";
    }


    QString data = QString::number(speed);

    return data+prefix;

}
