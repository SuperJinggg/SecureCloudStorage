#include "searchresultwidget.h"
#include "ui_searchresultwidget.h"

SearchResultWidget::SearchResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchResultWidget)
{
    ui->setupUi(this);
    if(nullptr==local_linedit){
        local_linedit = new Local_LineEdit();
        local_linedit->setValidator(new QIntValidator(1, 1000, this)); //1, 1000为输入的数字值范围
        connect(local_linedit,
                SIGNAL(signal_linedit_focussed(bool )),
                this,SLOT(slot_jumptopage_linedit_focussed(bool)));
        local_linedit->setText(QString::number(current_page+1));
        ui->jump_to_page_lineEdit_horizontalLayout->addWidget(local_linedit);
    }
    page_size = ui->show_num_comboBox->currentText().toInt();
    ui->tablewidget->setSortingEnabled(true);
    ui->tablewidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tablewidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tablewidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );//表格不可编辑
    ui->tablewidget->setRowCount(0);
//    ui->tablewidget->setColumnWidth(0, 40);  //设置列宽
    ui->tablewidget->setColumnWidth(0, 130);
    ui->tablewidget->setColumnWidth(1, 250);
    ui->tablewidget->setColumnWidth(2, 250);
    ui->tablewidget->setColumnWidth(3, 250);
    ui->tablewidget->setColumnWidth(4, 100);
}

void SearchResultWidget::data_search(
        QTabWidget *tabwidget,
        QString keywords,
        QString type,
        QString mode,
        QDateTime fromtime,
        QDateTime totime){
    this->tabwidget=tabwidget;
    this->keywords=keywords;
    this->type=type;
    this->mode=mode;
    this->fromtime=fromtime;
    this->totime=totime;

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
    network_request.setUrl(QUrl(url+"datasearch"));
    network_request.setRawHeader("sessionid", sessionid.toUtf8());
    network_request.setRawHeader("Content-Type", "application/json");
    //    QVariantList varList;
    QVariantMap var;
    var.insert("method", "datasearch");
    var.insert("version", "1.0");
    var.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    QVariantMap requestvar;
    QString condition = "AND";
    QStringList keywordslist;
    if(keywords.contains("|")){
        condition = "OR";
        keywordslist = keywords.split("|");
    }else{
        keywordslist = keywords.split("&");
    }
    requestvar.insert("condition", condition);
    if(0==this->mode.compare("filename")){
        requestvar.insert("mode", "filename");
        requestvar.insert("keywords",keywordslist);
    }else if(0==this->mode.compare("cipherindex")){
        requestvar.insert("mode", "cipherindex");
        QStringList cipherkeywordslist;
        for(int i = 0; i< keywordslist.size();++i)
        {
            QString tmp = keywordslist.at(i);
            string out = YunLock_EncryptKeyword(tmp.toStdString());
            cipherkeywordslist.append(QString::fromStdString(out));
        }
        requestvar.insert("keywords",cipherkeywordslist);
    }else{
        return;
    }
    if(0!=type.compare("")){
        requestvar.insert("type",type);
    }
    if(fromtime.isValid()&&totime.isValid()){
        QVariantMap timeregionvar;
        timeregionvar.insert("fromtime",fromtime.toString("yyyy-MM-dd HH:mm:ss"));
        timeregionvar.insert("totime",totime.toString("yyyy-MM-dd HH:mm:ss"));
        requestvar.insert("timeregion",timeregionvar);
    }
    requestvar.insert("pagenum",current_page);
    requestvar.insert("pagesize",page_size);
    var.insert("request",requestvar);

    QJsonObject obJct = QJsonObject::fromVariantMap(var);
    QJsonDocument jsonDoc(obJct);
    QByteArray json = jsonDoc.toJson();
    QString messagejsonstr(json);
    qDebug()<<Q_FUNC_INFO<<"messagejsonstr is "<<messagejsonstr;
    post_reply = net_manager->post(network_request,messagejsonstr.toUtf8());
    connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}

void SearchResultWidget::slot_jumptopage_linedit_focussed(bool stat){

}

void SearchResultWidget::slot_replyFinished(QNetworkReply* reply){

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
        this->setToolTip("检索结果");
        this->tabwidget->addTab(this,"检索结果");
        this->tabwidget->setCurrentWidget(this);
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
            filelist = parse_json_array(filelistarray);
            qDebug()<<Q_FUNC_INFO<<"filelist size is "<<filelist->size();

            ui->tablewidget->setRowCount(0);
            ui->tablewidget->setRowCount(filelist->size());

            QListIterator<FileMetadata*> iterater(*filelist);
            int addrow=0;
            while (iterater.hasNext()) {
                FileMetadata* entity =  iterater.next();
//                QTableWidgetItem *check=new QTableWidgetItem();
//                check->setCheckState (Qt::Unchecked);
//                entity->setcheckitem(check);
//                ui->tablewidget->setItem(addrow,0,check);
                OperationWidget *operationwidget = new OperationWidget();
                entity->setoperationwidget(operationwidget);
                ui->tablewidget->setCellWidget(addrow,0,entity->getoperationwidget());

                QTableWidgetItem *filename_item = new QTableWidgetItem();
                filename_item->setText(entity->getfilename());
                ui->tablewidget->setItem(addrow,1,filename_item);

                QTableWidgetItem *filesize_item = new QTableWidgetItem();
                filesize_item->setText(convert_size(entity->getfilesize()));
                ui->tablewidget->setItem(addrow,2,filesize_item);

                QTableWidgetItem *uploadtime_item = new QTableWidgetItem();
//                qDebug()<<Q_FUNC_INFO<<"uploadtime is "<<entity->getuploadtime();
                uploadtime_item->setText(entity->getuploadtime().toString("yyyy-MM-dd HH:mm:ss"));
                ui->tablewidget->setItem(addrow,3,uploadtime_item);

                QTableWidgetItem *type_item = new QTableWidgetItem();
                if(0==entity->gettype().compare("dir")){
                    type_item->setText("文件夹");
                }else{
                    type_item->setText("文件");
                }
                ui->tablewidget->setItem(addrow,4,type_item);
//                connect(entity, SIGNAL(signal_dataopen()),
//                        this, SLOT(slot_dataopen()));
//                connect(entity, SIGNAL(signal_datadownload()),
//                        this, SLOT(slot_datadownload()));
//                connect(entity, SIGNAL(signal_datashare()),
//                        this, SLOT(slot_datashare()));
//                connect(entity, SIGNAL(signal_datadelete()),
//                        this, SLOT(slot_datadelete()));
                addrow++;
            }
            ui->tablewidget->sortItems(4);

            ui->page_total_num_label->setText("共"+QString::number(((int)(filelist->size()/page_size))+1)+"页");
            ui->metadata_totalnum_label->setText("显示"+QString::number((current_page*page_size)+1)
                                                 +"-"+QString::number(filelist->size()-(current_page)*page_size)
                                                 +"，共"+QString::number(filelist->size())+"条记录");
        }
    }


}


QList<FileMetadata*> * SearchResultWidget::parse_json_array(QJsonArray &filelistarray ){

    QList<FileMetadata*> *childrenfolderstructure = new QList<FileMetadata*>();
    for(int i = 0; i< filelistarray.size(); i++)
    {
        FileMetadata* folderstruc = new FileMetadata();
        QJsonObject pobj =  filelistarray.at(i).toObject();
        int id;
        QString filename;
        int filesize;
        QString ctime;
        QString mtime;
        QString lastaccesstime;
        QString uploadtime;
        QString plaintexthash;
        QString ciphertexthash;
        int ciphertextsize;
        QString keyhash;
        QString fileuniqueid;
        QString type;
        QString currentid;
        QString parentid;
        bool ifshared;
        bool ifopened;

        if(pobj.contains("id"))
        {
            id = pobj.value("id").toInt();
        }
        if(pobj.contains("filename"))
        {
            filename = pobj.value("filename").toString();
            folderstruc->setfilename(filename);
        }
        if(pobj.contains("filesize"))
        {
            filesize = pobj.value("filesize").toInt();
            folderstruc->setfilesize(filesize);
        }
        if(pobj.contains("ctime"))
        {
            ctime = pobj.value("ctime").toString();
            QDateTime ctimedata = QDateTime::fromString(ctime, "yyyy-MM-dd HH:mm:ss");
            folderstruc->setctime(ctimedata);
        }
        if(pobj.contains("mtime"))
        {
            mtime = pobj.value("mtime").toString();
            QDateTime mtimedata = QDateTime::fromString(mtime, "yyyy-MM-dd HH:mm:ss");
            folderstruc->setmtime(mtimedata);
        }
        if(pobj.contains("uploadtime"))
        {
            uploadtime = pobj.value("uploadtime").toString();
            QDateTime uploadtimedata;

            uploadtimedata = QDateTime::fromString(uploadtime, "yyyy-MM-ddTHH:mm:ss");
//            qDebug()<<Q_FUNC_INFO<<"uploadtimedata is "<<uploadtimedata;
            folderstruc->setuploadtime(uploadtimedata);
        }
        if(pobj.contains("plaintexthash"))
        {
            plaintexthash = pobj.value("plaintexthash").toString();
            folderstruc->setplaintexthash(plaintexthash);
        }
        if(pobj.contains("ciphertexthash"))
        {
            ciphertexthash = pobj.value("ciphertexthash").toString();
            folderstruc->setcipherhash(ciphertexthash);
        }
        if(pobj.contains("keyhash"))
        {
            keyhash = pobj.value("keyhash").toString();
            folderstruc->setkeyhash(keyhash);
        }
        if(pobj.contains("fileuniqueid"))
        {
            fileuniqueid = pobj.value("fileuniqueid").toString();
            folderstruc->setfileuniqueid(fileuniqueid);
        }
        if(pobj.contains("type"))
        {
            type = pobj.value("type").toString();
            folderstruc->settype(type);
        }
        if(pobj.contains("currentid"))
        {
            currentid = pobj.value("currentid").toString();
            folderstruc->setcurrentid(currentid);
        }
        if(pobj.contains("parentid"))
        {
            parentid = pobj.value("parentid").toString();
            folderstruc->setparentid(parentid);
        }
        if(pobj.contains("ifshared"))
        {
            ifshared = pobj.value("ifshared").toBool();
            folderstruc->setifsheared(ifshared);
        }
        if(pobj.contains("ifopened"))
        {
            ifopened = pobj.value("ifopened").toBool();
            folderstruc->setifopened(ifopened);
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
        childrenfolderstructure->append(folderstruc);
    }
    return childrenfolderstructure;
}
QString SearchResultWidget::convert_size(qint64 size){
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
void SearchResultWidget::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void SearchResultWidget::slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}

void SearchResultWidget::slot_NetWorkError(QNetworkReply::NetworkError){

}

SearchResultWidget::~SearchResultWidget()
{
    delete ui;
}
