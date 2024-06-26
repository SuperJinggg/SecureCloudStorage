#ifndef DOWNLOADFILEENTITY_H
#define DOWNLOADFILEENTITY_H

#include <QObject>
#include "./downloadoperation.h"
#include "../transformlistform/downloadlistform.h"

class DownloadFileEntity : public QObject
{
    Q_OBJECT
public:
    explicit DownloadFileEntity(QObject *parent = nullptr);
    explicit DownloadFileEntity(FileMetadata* downloadfilemetadata,QObject *parent = nullptr);

    QListWidgetItem *getdownloaditem() {return this->downloaditem;}
    DownloadListForm *getdownloadlistform(){return this->downloadlistform;}

    void start();
signals:

    void signal_downloadopeartion_begin();

public slots:

private:
    FileMetadata* downloadfilemetadata{downloadfilemetadata=nullptr};
    QListWidgetItem *downloaditem {downloaditem = nullptr};
    DownloadListForm *downloadlistform{downloadlistform=nullptr};
    DownloadOperation *downloadoperation {downloadoperation=nullptr};
    QThread * datadownloadthread{datadownloadthread = nullptr};
};

#endif // DOWNLOADFILEENTITY_H
