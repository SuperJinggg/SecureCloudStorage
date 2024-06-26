#include "filemetadata.h"

FileMetadata::FileMetadata(QObject *parent) : QObject(parent)
{

}

QString FileMetadata::getfilename(){
     return this->filename;
}

long FileMetadata::getfilesize(){
     return this->filesize;
}

QDateTime FileMetadata::getctime(){
    return this->ctime;
}

QDateTime FileMetadata::getmtime(){
    return this->mtime;
}

QDateTime FileMetadata::getuploadtime(){
    return this->uploadtime;
}

QString FileMetadata::getfileowner(){
    return this->fileowner;
}

QString FileMetadata::getplaintexthash(){
    return this->plaintexthash;
}

QString FileMetadata::getcipherhash(){
    return this->cipherhash;
}

QString FileMetadata::getkeyhash(){
    return this->keyhash;
}

QString FileMetadata::getfileuniqueid(){
    return this->fileuniqueid;
}

QString FileMetadata::gettype(){
     return this->type;
}

QString FileMetadata::getcurrentid(){
    return this->currentid;
}

QString FileMetadata::getparentid(){
     return this->parentid;
}

bool FileMetadata::getifsheared(){
    return this->ifsheared;
}

QString FileMetadata::getsavepath(){
    return this->savepath;
}

void FileMetadata::setfilename(QString filename){
    this->filename=filename;
}

void FileMetadata::setfilesize(long filesize){
    this->filesize=filesize;
}

void FileMetadata::setctime(QDateTime ctime){
    this->ctime=ctime;

}
void FileMetadata::setmtime(QDateTime mtime){
    this->mtime=mtime;
}

void FileMetadata::setuploadtime(QDateTime uploadtime){
    this->uploadtime=uploadtime;
}

void FileMetadata::setfileowner(QString fileowner){
    this->fileowner=fileowner;
}

void FileMetadata::setplaintexthash(QString plaintexthash){
    this->plaintexthash=plaintexthash;
}

void FileMetadata::setcipherhash(QString cipherhash){
    this->cipherhash=cipherhash;
}

void FileMetadata::setkeyhash(QString keyhash){
    this->keyhash=keyhash;
}

void FileMetadata::setfileuniqueid(QString fileuniqueid){
    this->fileuniqueid=fileuniqueid;
}

void FileMetadata::settype(QString type){
    this->type=type;
}

void FileMetadata::setsavepath(QString savepath){
    this->savepath=savepath;
}


void FileMetadata::setcurrentid(QString currentid){
    this->currentid=currentid;
}

void FileMetadata::setparentid(QString parentid){
    this->parentid=parentid;
}

void FileMetadata::setifsheared(bool ifsheared){
    this->ifsheared=ifsheared;
}

bool FileMetadata::getifopened(){
    return this->ifopened;
}
void FileMetadata::setifopened(bool ifopened){
    this->ifopened=ifopened;
}
QList<FileMetadata * > *FileMetadata::getchildren(){
    return this->children;
}

void FileMetadata::setchildren(QList<FileMetadata * > *children){
    this->children=children;
}
OperationWidget * FileMetadata::getoperationwidget(){
    return this->operationwidget;
}

void FileMetadata::setoperationwidget(OperationWidget *operationwidget){
    this->operationwidget=operationwidget;

    connect(this->operationwidget, SIGNAL(signal_dataopen()),
            this, SLOT(slot_dataopen()));
    connect(this->operationwidget, SIGNAL(signal_datadownload()),
            this, SLOT(slot_datadownload()));
    connect(this->operationwidget, SIGNAL(signal_datashare()),
            this, SLOT(slot_datashare()));
    connect(this->operationwidget, SIGNAL(signal_datadelete()),
            this, SLOT(slot_datadelete()));


}


QTableWidgetItem * FileMetadata::getcheckitem(){
    return this->checkitem;
}
void FileMetadata::setcheckitem(QTableWidgetItem *checkitem){
    this->checkitem=checkitem;
}


void FileMetadata::slot_dataopen(){
    qDebug()<<Q_FUNC_INFO<<"slot_dataopen";
    emit signal_dataopen();
}

void FileMetadata::slot_datadownload(){
    qDebug()<<Q_FUNC_INFO<<"slot_datadownload";
    emit signal_datadownload();
}

void FileMetadata::slot_datashare(){
    emit signal_datashare();
}

void FileMetadata::slot_datadelete(){
    emit signal_datadelete();
}
