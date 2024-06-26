#include "downloadfileentity.h"

DownloadFileEntity::DownloadFileEntity(QObject *parent) : QObject(parent)
{

}

DownloadFileEntity::DownloadFileEntity(FileMetadata* downloadfilemetadata,QObject *parent) : QObject(parent)
{

    this->downloadfilemetadata=downloadfilemetadata;

    if(nullptr==downloadoperation){
        downloadoperation = new DownloadOperation(file_path,this->downloadfilemetadata);
        connect(this, SIGNAL(signal_downloadopeartion_begin()),
                downloadoperation, SLOT(slot_data_opeartion()));

    }

    if(nullptr==downloaditem){
        downloaditem = new QListWidgetItem();
    }
    if(nullptr==downloadlistform){
        downloadlistform = new DownloadListForm(this->downloadfilemetadata);
        connect(downloadoperation, SIGNAL(setstat(QString)),
                downloadlistform, SLOT(setstat(QString)));
        connect(downloadoperation, SIGNAL(toSetProgressBar(int)),
                downloadlistform, SLOT(setprogress(int)));
        connect(downloadlistform, SIGNAL(signal_decrypt_and_open_file()),
                downloadoperation, SLOT(slot_decrypt_and_open_file()));

    }

    if (nullptr == datadownloadthread) {
        datadownloadthread = new QThread();
    }
    if(nullptr!=downloadoperation){
        downloadoperation->DoSetup(*datadownloadthread);
        downloadoperation->moveToThread(datadownloadthread);
    }else{

    }

}
void DownloadFileEntity::start(){
    if (!datadownloadthread->isRunning()) {
        datadownloadthread->start();
    }
    else {
        //send signal
        emit signal_downloadopeartion_begin();
    }
}

