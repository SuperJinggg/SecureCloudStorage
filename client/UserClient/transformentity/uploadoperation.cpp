#include "uploadoperation.h"

UploadOperation::UploadOperation(QObject *parent) : QObject(parent)
{

}
UploadOperation::UploadOperation(
        QString parentid,
        QString plainfile,
        QString cipherfile,
        QObject *parent) : QObject(parent)
{
    this->parentid=parentid;
    this->plainfile=plainfile;
    this->cipherfile=cipherfile;

}

void UploadOperation::DoSetup(QThread &cThread) {
    connect(&cThread, SIGNAL(started()), this, SLOT(slot_data_opeartion()));
}

void UploadOperation::slot_data_opeartion(){
    encryptfile();
    generateMetadata();
    generateCipherIndex();

}

void UploadOperation::encryptfile(){
    emit setstat("正在加密");
    QString ciphertextpath = this->cipherfile;
    qDebug()<<"plaintextpath ="<< this->plainfile;
    QString uuid = QUuid::createUuid().toString().replace("{","").replace("}","").replace("-","");
    QString uuid2 = QUuid::createUuid().toString().replace("{","").replace("}","").replace("-","");
    this->encryptkey = uuid+uuid2;
    string encrypt_key =encryptkey.toStdString();
    qDebug()<<"random_str ="<< encryptkey;
    string encrypt_ret =  YunLock_EncryptFile
            (this->plainfile.toStdString(),
             ciphertextpath.toStdString(),
             encrypt_key);
    encrypt_retqstr = QString::fromStdString(encrypt_ret);
    qDebug()<<"encrypt_retqstr ="<< encrypt_retqstr;
    QString function;
    QString result;
    QString fileuniqueid;
    QString response_time;
    QString code;
    QString note;
    QByteArray resultjsonbytearray;
    resultjsonbytearray.append(encrypt_retqstr);
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
                if(iteraterkey=="function"){
                    function = iteratervalue.toString();
                }else if(iteraterkey=="Result"){
                    result = iteratervalue.toString();
                }
                else if(iteraterkey=="file_unique_id"){
                    fileuniqueid = iteratervalue.toString();
                }
                else if(iteraterkey=="Response_time"){
                    response_time = iteratervalue.toString();
                }
                else if(iteraterkey=="Code"){
                    code = iteratervalue.toString();
                }
                else if(iteraterkey=="Note"){
                    note = iteratervalue.toString();
                }else{

                }

            }

        }
    }else{
        qDebug() << Q_FUNC_INFO << "解析json失败";

    }
    if(0==result.compare("success")){
        this->file_unique_id = fileuniqueid;
        emit setstat("加密完成");
    }
}

void UploadOperation::generateCipherIndex(){
    QString frompath = this->plainfile;
    if (frompath.endsWith(".txt") ||
            frompath.endsWith(".doc", Qt::CaseInsensitive) ||
            frompath.endsWith(".docx", Qt::CaseInsensitive) ||
            frompath.endsWith(".ppt", Qt::CaseInsensitive) ||
            frompath.endsWith(".pptx", Qt::CaseInsensitive) ||
            frompath.endsWith(".xlsx", Qt::CaseInsensitive) ||
            frompath.endsWith(".xls", Qt::CaseInsensitive) ||
            frompath.endsWith(".pdf", Qt::CaseInsensitive) ||
            frompath.endsWith(".html", Qt::CaseInsensitive) ||
            frompath.endsWith(".rtf", Qt::CaseInsensitive) ||
            frompath.endsWith(".odt", Qt::CaseInsensitive) ||
            frompath.endsWith(".ods", Qt::CaseInsensitive) ||
            frompath.endsWith(".odp", Qt::CaseInsensitive)||
            frompath.endsWith(".html", Qt::CaseInsensitive)||
            frompath.endsWith(".odg", Qt::CaseInsensitive)
            ) {
        emit setstat("正在生成密文索引");
        QString configPath;
        QDir dir;
        configPath = dir.currentPath();
        QString doctotextpath = configPath + "/doctotext/doctotext.exe";
        QProcess * _mprogress = new QProcess();
        connect(_mprogress, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(slot_qprogress_finished(int, QProcess::ExitStatus)));
        connect(_mprogress, SIGNAL(readyReadStandardOutput()),
                this, SLOT(slot_qprogress_readyReadStandardOutput()));
        connect(_mprogress, SIGNAL(started()),
                this, SLOT(slot_qprogress_started()));
        qDebug() << "doctotextpath is " <<doctotextpath;
        qDebug() << "frompath is " <<frompath;
        _mprogress->start(doctotextpath ,QStringList(frompath));
        qDebug() << "Let's Go";
    }else{
        uploaddata();
    }

}

void UploadOperation::slot_qprogress_readyReadStandardOutput() {
    qDebug() << "readyOut";
    QProcess *p = (QProcess *)sender();
    QByteArray buf = p->readAllStandardOutput();
    QString docout = QString(buf);
    doc_plaintext = doc_plaintext + docout;
    //    YunLock_UploadCipherIndex();
}

void UploadOperation::slot_qprogress_started() {
    qDebug() << "Proc Started";
}

void UploadOperation::slot_qprogress_finished(int exitCode, QProcess::ExitStatus exitStatus) {
    qDebug() << "Finished: " << exitCode <<"doc_plaintext length is "<<doc_plaintext.length();

    if(nullptr!=doc_plaintext&&doc_plaintext.length()>0){
        string cipherindex = YunLock_GenerateCipherIndex(
                    doc_plaintext.toStdString(),
                    this->file_unique_id.toStdString());

        this->encrypted_index = QString::fromStdString(cipherindex).trimmed().simplified().remove(QRegExp("\\s"));
        qDebug()<<Q_FUNC_INFO<<encrypted_index;
        emit setstat("密文索引生成完成");
    }

    uploaddata();
}


void UploadOperation::generateMetadata(){
    emit setstat("正在生成数据元信息");
    QFileInfo fileinfo(this->plainfile);
    QString filename = fileinfo.fileName();
    long filesize = fileinfo.size();
    QString ctime = fileinfo.created().toString("yyyy-MM-dd hh:mm:ss");
    QString mtime = fileinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    QString lastaccesstime = fileinfo.lastRead().toString("yyyy-MM-dd hh:mm:ss");
    QString uploadtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString fileowner = username;
    QString plaintexthash = calcfilehash(this->plainfile,"sha256");
    QString ciphertexthash = calcfilehash(this->cipherfile,"sha256");
    QString encryptkeyhash = calcstrhash(this->encryptkey);
    QFileInfo cipherfileinfo(this->cipherfile);
    long ciphertextsize = cipherfileinfo.size();
    QString type = fileinfo.isFile()?"file":"dir";
    QString currentid = QUuid::createUuid().toString().replace("{","").replace("}","").replace("-","");

    //    qDebug()<<"filename is "<<filename
    //           <<"filesize is "<<filesize
    //          <<"ctime is "<<ctime.toString("yyyy-MM-dd hh:mm:ss")
    //         <<"mtime is "<<mtime.toString("yyyy-MM-dd hh:mm:ss")
    //        <<"lastaccesstime is "<<lastaccesstime.toString("yyyy-MM-dd hh:mm:ss")
    //       <<"uploadtime is "<<uploadtime.toString("yyyy-MM-dd hh:mm:ss")
    //      <<"fileowner is "<<fileowner
    //     <<"plaintexthash is "<<plaintexthash
    //    <<"ciphertexthash is "<<ciphertexthash
    //    <<"encryptkeyhash is "<<encryptkeyhash
    //    <<"ciphertextsize is "<<ciphertextsize
    //    <<"type is "<<type
    //    <<"currentid is "<<currentid;


    QVariantMap metadatajsonvar; //初始化QVariantMap对象
    /*…添加其他数据元信息*/
    metadatajsonvar.insert("filename", filename);//设置filename值
    metadatajsonvar.insert("filesize", QString::number(filesize));//设置filesize值
    metadatajsonvar.insert("ctime", ctime);
    metadatajsonvar.insert("mtime", mtime);
    metadatajsonvar.insert("lastaccesstime", lastaccesstime);
    metadatajsonvar.insert("uploadtime", uploadtime);
    metadatajsonvar.insert("fileowner", fileowner);
    metadatajsonvar.insert("plaintexthash", plaintexthash);
    metadatajsonvar.insert("ciphertexthash", ciphertexthash);
    metadatajsonvar.insert("ciphertextsize", QString::number(ciphertextsize));
    metadatajsonvar.insert("encryptkeyhash", encryptkeyhash);
    metadatajsonvar.insert("fileuniqueid", this->file_unique_id);
    metadatajsonvar.insert("type", type);
    metadatajsonvar.insert("currentid", currentid);
    metadatajsonvar.insert("parentid", parentid);

    QJsonObject obJct =QJsonObject::fromVariantMap(metadatajsonvar); //QVariantMap转QJsonObject
    QJsonDocument jsonDoc(obJct);//QJsonObject转QJsonDocument
    QByteArray json=jsonDoc.toJson();//QJsonDocument转QByteArray
    QString messagejsonstr(json);//QByteArray 转QString

    metadata = messagejsonstr.trimmed().simplified();
    emit setstat("数据元信息生成完成");

}

QString UploadOperation::calcfilehash(QString path, QString alg_method) {

    FILE *from_fd = nullptr;
    char outcome[65] = "\0";
    int i = 0;
    //获取文件大小
    QFile localfile(path);
    const char* from_file_byte =nullptr;
    QByteArray ba = path.toLocal8Bit();
    from_file_byte = strdup(ba.data());
    unsigned char in[BUFFER_SIZE] = "\0";
    int bytes_read = 0;
    unsigned char out[33] = "\0";
    from_fd = fopen(from_file_byte, "rb");
    if (from_fd == nullptr) {
        return nullptr;
    }
    if (alg_method == "sha256") {
        sha256_context context;
        sha256_starts(&context);
        while (bytes_read = fread(in, sizeof(char), BUFFER_SIZE, from_fd)) {
            if ((bytes_read == -1) && (errno != EINTR)) {
                break;
            }
            else if (bytes_read == BUFFER_SIZE) {
                sha256_update(&context, in, bytes_read);
            }
            else if ((bytes_read < BUFFER_SIZE)) {
                sha256_update(&context, in, bytes_read);
            }
            else {
                sha256_update(&context, in, bytes_read);
            }
        }
        sha256_finish(&context , out);
        for (i = 0; i < 32; i++) {
            sprintf(outcome + 2 * i, "%02x", (unsigned char)out[i]);
        }
    }

    if (fclose(from_fd) == EOF) {
    }
    return QString(outcome);
}

QString UploadOperation::calcstrhash(QString input){
    char outcome[65] = "\0";
    unsigned char digest[32]="\0";
    unsigned char* inputbyte = (unsigned char*)(input.toUtf8().data());
    int length = input.length();
    sha256(digest,inputbyte,length);
    int i = 0;
    for (i = 0; i < 32; i++) {
        sprintf(outcome + 2 * i, "%02x", (unsigned char)digest[i]);
    }
    return QString(outcome);
}

char* UploadOperation::getTextStr(QString string)
{
    QString str = string;
    char* text = nullptr;
    QByteArray ba = string.toLocal8Bit();
    //text = ba.data();      // ba是个局部变量，用完就析构了，所以后面再取text就出现乱码
    text = strdup(ba.data()); //直接拷贝出来就不会乱码了
    return text;
}


void UploadOperation::uploaddata(){

    emit setstat("密文数据正在上传");

    if (nullptr == manager) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(slot_replyFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(slot_provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }
    QHttpMultiPart *mMultiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    QFile *file = new QFile(this->cipherfile);
    QFileInfo fileinfo(this->cipherfile);
    file->open(QIODevice::ReadOnly);
    QByteArray sessinkeyarray = sessionkey.toUtf8();
    char * key = sessinkeyarray.data();
    int keylen = sessinkeyarray.length();
    QByteArray encryptkeybytearray = this->encryptkey.toUtf8();
    char* input = encryptkeybytearray.data();
    int inputlen = encryptkeybytearray.length();
    char cipherkeyout[109] = "\0";
    aes_cbc_pkcs5_encrypt(key,keylen,input,inputlen,cipherkeyout);
    QString outencryptkey(cipherkeyout);

    qDebug()<<Q_FUNC_INFO<<"sessionkey is "<<sessionkey
           <<"keylen is "<<keylen
             <<"encryptkey is "<<encryptkey
               <<"inputlen is "<<inputlen
                 <<"outencryptkey is "<<outencryptkey;

    QString contentheadstr="form-data; name=\"file\"; filename=\""+fileinfo.fileName()+"\"";
    QString contenttypestr= "application/octet-stream";
    long headersize =
            contentheadstr.length()+
            sessionid.toUtf8().length()+
            this->metadata.toUtf8().length()+
            this->file_unique_id.toUtf8().length()+
            this->encrypted_index.toUtf8().length()+
            outencryptkey.toUtf8().length();

    qDebug()<<Q_FUNC_INFO<<"headersize is "<<headersize;

    if(headersize<10000){//因为当头部超过10240的时候，服务端会有限制，所以这里限制头部为小于10000
        filePart.setBodyDevice(file);//这里的file对象就是我们需要提交的文件，如果需要上传多个文件，我们就需要创建多个QHttpPart对象
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(contenttypestr));//这里我们上传的是图片
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant(contentheadstr));
        filePart.setRawHeader("metadata",this->metadata.toUtf8());
        filePart.setRawHeader("fileuniqueid",this->file_unique_id.toUtf8());
        filePart.setRawHeader("encrypted_index",this->encrypted_index.toUtf8());
        filePart.setRawHeader("encryptkey",outencryptkey.toUtf8());
        mMultiPart->append(filePart);
        QUrl uploaddataurl(url+"dataupload");
        QNetworkRequest request(uploaddataurl);
        request.setRawHeader("sessionid", sessionid.toUtf8());
        post_reply = manager->post(request,mMultiPart);
        connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
        connect(post_reply, SIGNAL(uploadProgress(qint64, qint64)), this,
                SLOT(progressChanged(qint64, qint64)));
    }else{//当头部大于10240的时候，文件和元信息分别上传

        filePart.setBodyDevice(file);//这里的file对象就是我们需要提交的文件，如果需要上传多个文件，我们就需要创建多个QHttpPart对象
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(contenttypestr));//这里我们上传的是图片
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant(contentheadstr));
        filePart.setRawHeader("fileuniqueid",this->file_unique_id.toUtf8());
        mMultiPart->append(filePart);
        QUrl uploaddataurl(url+"singledataupload");
        QNetworkRequest request(uploaddataurl);
        request.setRawHeader("sessionid", sessionid.toUtf8());
        post_reply = manager->post(request,mMultiPart);
        connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
        connect(post_reply, SIGNAL(uploadProgress(qint64, qint64)), this,
                SLOT(progressChanged(qint64, qint64)));
        uploadotherdata(
                    sessionid,
                    this->metadata,
                    this->file_unique_id,
                    this->encrypted_index,
                    outencryptkey
                    );

    }
}

void UploadOperation::uploadotherdata(QString sessionid,
                                      QString metadata,
                                      QString file_unique_id,
                                      QString encrypted_index,
                                      QString outencryptkey
                                      ){

    if (nullptr == otherdatamanager) {
        otherdatamanager = new QNetworkAccessManager(this);
        connect(otherdatamanager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(slot_otherdata_replyFinished(QNetworkReply*)));
        connect(otherdatamanager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_otherdata_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(otherdatamanager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(slot_otherdata_provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }

    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_2);


    QUrl uploaddataurl(url+"otherdataupload");
    QNetworkRequest request(uploaddataurl);
    request.setSslConfiguration(config);
    request.setRawHeader("sessionid", sessionid.toUtf8());
    request.setRawHeader("Content-Type", "application/json");
    QVariantMap var;
    var.insert("method", "otherdataupload");
    var.insert("version", "1.0");
    var.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    QVariantMap requestvar;

    requestvar.insert("metadata", metadata);
    requestvar.insert("fileuniqueid", file_unique_id);
    requestvar.insert("encryptedindex", encrypted_index);
    requestvar.insert("outencryptkey", outencryptkey);

    var.insert("request",requestvar);

    QJsonObject obJct = QJsonObject::fromVariantMap(var);
    QJsonDocument jsonDoc(obJct);
    QByteArray json = jsonDoc.toJson();
    QString messagejsonstr(json);
    qDebug()<<Q_FUNC_INFO<<"messagejsonstr is "<<messagejsonstr;
    otherdatapost_reply = otherdatamanager->post(request,messagejsonstr.toUtf8());
    connect(otherdatapost_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slot_otherdata_NetWorkError(QNetworkReply::NetworkError)));
}

void UploadOperation::slot_replyFinished(QNetworkReply* reply){
    qDebug() << Q_FUNC_INFO << "in slot_replyFinished";
    QString ret_data;
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug()<<Q_FUNC_INFO << "status code=" << statusCode.toInt();
    QString method="";
    QString result="";
    QString code = "";
    QString message = "";
    QVariantMap details;
    QString username;
    if (nullptr != reply) {
        ret_data = reply->readAll();
        qDebug() << Q_FUNC_INFO << "reply readAll is " << ret_data;



    }

}

void UploadOperation::progressChanged(qint64 progress, qint64 total){

    qDebug()<<"progress is " << progress
           <<" total is " << total;

    if (0!=total) {
        int progressnum = ((int)(progress * 100 / total));
        qDebug()<< Q_FUNC_INFO <<progressnum;
        emit toSetProgressBar(progressnum);
        if(100<=progressnum){
            emit setstat("上传成功");
        }else{
            emit setstat("密文数据正在上传");
        }
    }
}


void UploadOperation::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    qDebug() << Q_FUNC_INFO << "in slot_sslErrors";
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void UploadOperation::slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator) {

}

void UploadOperation::slot_NetWorkError(QNetworkReply::NetworkError errorCode) {
    qDebug() << Q_FUNC_INFO << "in slot_NetWorkError";
    if (nullptr != post_reply) {
        post_reply->deleteLater();
        post_reply = nullptr;
    }
    if (nullptr != manager) {
        delete manager;
        manager = nullptr;
    }
}


void UploadOperation::slot_otherdata_replyFinished(QNetworkReply* reply){

    qDebug() << Q_FUNC_INFO << "in slot_otherdata_replyFinished";
    QString ret_data;
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug()<<Q_FUNC_INFO << "status code=" << statusCode.toInt();
    QString method="";
    QString result="";
    QString code = "";
    QString message = "";
    QVariantMap details;
    QString username;
    if (nullptr != reply) {
        ret_data = reply->readAll();
        qDebug() << Q_FUNC_INFO << "reply readAll is " << ret_data;



    }
}

void UploadOperation::slot_otherdata_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void UploadOperation::slot_otherdata_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}

void UploadOperation::slot_otherdata_NetWorkError(QNetworkReply::NetworkError){
    if (nullptr != otherdatapost_reply) {
        otherdatapost_reply->deleteLater();
        otherdatapost_reply = nullptr;
    }
    if (nullptr != otherdatamanager) {
        delete otherdatamanager;
        otherdatamanager = nullptr;
    }
}


void UploadOperation::aes_cbc_pkcs5_encrypt(char* key , int keylen , char* input, int inputlen, char* pcOut)
{
    char iv[17] = "\0";
    AES_KEY AES;
    int blocknum = inputlen / AES_BLOCK_SIZE + 1;
    int totallength = blocknum * AES_BLOCK_SIZE;
    char *plaintext = (char*)malloc(totallength+1);
    char *ciphertext = (char*)malloc(totallength+1);;
    int paddingvalue = 0;
    if (inputlen % AES_BLOCK_SIZE > 0)
    {
        paddingvalue = totallength - inputlen;
    }else{
        paddingvalue = 16;
    }

    memset(plaintext, paddingvalue, totallength);
    memcpy(plaintext, input, inputlen);
    if (AES_set_encrypt_key((unsigned char*)key, keylen*8, &AES) < 0)
    {
        fprintf(stderr, "Unable to set encryption key in AES\n");
        exit(-1);
    }
    AES_cbc_encrypt((unsigned char *)plaintext,
                    (unsigned char*)ciphertext,
                    totallength,
                    &AES,
                    (unsigned char*)iv,
                    AES_ENCRYPT);
    base64enc(pcOut , ciphertext, totallength);
    if(nullptr!=plaintext){
       free(plaintext);
    }
    if(nullptr!=ciphertext){
       free(ciphertext);
    }
}


