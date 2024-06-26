#include "folderstructurelist.h"

FolderStructureList::FolderStructureList(QObject *parent) : QObject(parent)
{

}




FolderStructureList::FolderStructureList(int current_page,int  page_size,QTreeWidget *treewidget,QObject *parent): QObject(parent){
    this->current_page=current_page;
    this->page_size=page_size;
    this->treewidget=treewidget;
    this->treewidget->setHeaderHidden(true);
    if(nullptr==folderstructure){
        folderstructure = new QList<FileMetadata*>();
    }

    getfolderstructure();
}


void FolderStructureList::getfolderstructure(){
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
    network_request.setUrl(QUrl(url+"folderstructurelist"));
    network_request.setRawHeader("sessionid", sessionid.toUtf8());
    network_request.setRawHeader("Content-Type", "application/json");
    //    QVariantList varList;
    QVariantMap var;
    var.insert("method", "folderstructurelist");
    var.insert("version", "1.0");
    var.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    //    JsonObject messagejsonobj;
    //    messagejsonobj["method"]="folderstructurelist";
    //    messagejsonobj["version"]="1.0";
    //    messagejsonobj["timestamp"]=QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QJsonObject obJct = QJsonObject::fromVariantMap(var);
    QJsonDocument jsonDoc(obJct);
    QByteArray json = jsonDoc.toJson();
    QString messagejsonstr(json);
    //    messagejsonstr = QtJson::serialize(messagejsonobj);
//    qDebug()<<Q_FUNC_INFO<<"messagejsonstr is "<<messagejsonstr;
    post_reply = net_manager->post(network_request,messagejsonstr.toUtf8());
    connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}

void FolderStructureList::folderstructurelist(
        QTreeWidgetItem *top_item,
        QList<FileMetadata*> *folderstructure){
    QListIterator<FileMetadata*> iterater(*folderstructure);
    while (iterater.hasNext()) {
        FileMetadata* entity =  iterater.next();
        QStringList strlist;
        strlist.append(entity->getfilename());
        LocalTreeWidgetItem* item = new LocalTreeWidgetItem(strlist);
        item->setfilemetadata(entity);
        top_item->addChild(item);
        if(item->getfilemetadata()->getifopened()){
            item->setExpanded(true);
        }

        if(nullptr!=entity->getchildren()&&entity->getchildren()->size()>0){
            folderstructurelist(item,entity->getchildren());
        }
    }
}

void FolderStructureList::slot_replyFinished(QNetworkReply* reply){
    QByteArray ret_data;
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid()){

    }
//        qDebug() <<Q_FUNC_INFO<< "status code=" << statusCode.toInt();
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
        QJsonArray folderstruarray;
        if(detailsobj.contains("folderstructure")){
            folderstruarray = detailsobj.value("folderstructure").toArray();
            folderstructure = parse_json_array(folderstruarray);
        }

//        for(int i = 0 ;i<10;i++){
//            FileMetadata* folder = new FileMetadata();
//            QList<FileMetadata*> *childrenfolderstructure = new QList<FileMetadata*>();
//            for(int ci = 0 ;ci<2;ci++){
//                FileMetadata* childrenfolder = new FileMetadata();
//                childrenfolder->setfilename("Child Folder "+ QString::number(ci));
//                childrenfolder->setcurrentid("Child Folder cid "+ QString::number(ci));
//                childrenfolderstructure->append(childrenfolder);
//            }

//            folder->setfilename("New Folder "+ QString::number(i));
//            folder->setchildren(childrenfolderstructure);
//            folder->setcurrentid("New Folder cid"+ QString::number(i));
//            folderstructure->append(folder);
//        }

    }

    qDebug()<<folderstructure->size();


    QStringList top_name;
    top_name.append("根目录");
    LocalTreeWidgetItem *top_item = new LocalTreeWidgetItem(top_name);
    top_item->setfilemetadata(nullptr);
    delete this->treewidget->takeTopLevelItem(0);
    this->treewidget->addTopLevelItem(top_item);
    this->treewidget->expandItem(top_item);
    this->treewidget->setFocus();
    this->treewidget->setCurrentItem(top_item);
    folderstructurelist(top_item,folderstructure);


    emit itemPressed((QTreeWidgetItem*)top_item,1);

}


QList<FileMetadata*> * FolderStructureList::parse_json_array(QJsonArray &folderstruarray ){

    QList<FileMetadata*> *childrenfolderstructure = new QList<FileMetadata*>();
    for(int i = 0; i< folderstruarray.size(); i++)
    {
        FileMetadata* folderstruc = new FileMetadata();

        QJsonObject pobj =  folderstruarray.at(i).toObject();
        int id;
        QString currentid;
        bool ifopened;
        QString foldername;

        if(pobj.contains("id"))
        {
            id = pobj.value("id").toInt();

        }
        if(pobj.contains("currentid"))
        {
            currentid = pobj.value("currentid").toString();
            folderstruc->setcurrentid(currentid);
        }
        if(pobj.contains("opened"))
        {
            ifopened = pobj.value("opened").toBool();
            folderstruc->setifopened(ifopened);
        }

        if(pobj.contains("foldername"))
        {
            foldername = pobj.value("foldername").toString();
            folderstruc->setfilename(foldername);
        }
        if(pobj.contains("children"))
        {
            QJsonArray children =  pobj.value("children").toArray();
            QList<FileMetadata*> * childrenout = parse_json_array(children);
//            QJsonArray children = pobj.value("children").toArray();
            folderstruc->setchildren(childrenout);
        }
        childrenfolderstructure->append(folderstruc);

    }
    return childrenfolderstructure;
}


void FolderStructureList::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void FolderStructureList::slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator) {

}

void FolderStructureList::slot_NetWorkError(QNetworkReply::NetworkError errorCode) {


    if (nullptr != post_reply) {
        post_reply->deleteLater();
        post_reply = nullptr;
    }
    if (nullptr != net_manager) {
        delete net_manager;
        net_manager = nullptr;
    }
}
