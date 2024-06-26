#include "uploadfileentity.h"

UploadFileEntity::UploadFileEntity(QObject *parent) : QObject(parent)
{

}


UploadFileEntity::UploadFileEntity(QString filepath,QString parentid, QObject *parent) : QObject(parent)
{
    this->filepath=filepath;
    this->parentid=parentid;

    QFileInfo plainfile(this->filepath);
    QString configPath;
    QDir dir;
    configPath = dir.currentPath();
    QString cipherpath = configPath+"/encryptfile/";
    QDir cipherpathdir;
    if (!cipherpathdir.exists(cipherpath))
    {
        bool res = cipherpathdir.mkpath(cipherpath);
        qDebug() << "新建目录是否成功" << res;
    }

    if(nullptr==uploadoperation){
        uploadoperation = new UploadOperation(this->parentid,
                                              this->filepath,
                                              cipherpath+plainfile.fileName());
        connect(this, SIGNAL(signal_uploadopeartion_begin()),
                uploadoperation, SLOT(slot_data_opeartion()));

    }

    if(nullptr==uploaditem){
        uploaditem = new QListWidgetItem();
    }
    if(nullptr==uploadlistform){
        uploadlistform = new UploadListForm(this->filepath);
        connect(uploadoperation, SIGNAL(setstat(QString)),
                uploadlistform, SLOT(setstat(QString)));
        connect(uploadoperation, SIGNAL(toSetProgressBar(int)),
                uploadlistform, SLOT(setprogress(int)));
    }

    if (nullptr == datauploadthread) {
        datauploadthread = new QThread();
    }
    if(nullptr!=uploadoperation){
        uploadoperation->DoSetup(*datauploadthread);
        uploadoperation->moveToThread(datauploadthread);
    }else{

    }
}


void UploadFileEntity::start(){
    if (!datauploadthread->isRunning()) {
        datauploadthread->start();
    }
    else {
        //send signal
        emit signal_uploadopeartion_begin();
    }
}






