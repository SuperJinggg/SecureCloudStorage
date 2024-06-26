#ifndef UPLOADFILEENTITY_H
#define UPLOADFILEENTITY_H

#include <QObject>
#include <QProcess>
#include <QDir>
#include <QUuid>
#include <QDebug>
#include <QDateTime>
#include <QHttpMultiPart>
#include <QList>
#include <QFile>
#include <QVariant>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QThread>
#include <QListWidgetItem>


#include "../lib/include/cipherinterface.h"
#include "../global.h"
#include "../inc/sha256.h"
#include "../transformlistform/uploadlistform.h"
#include "./uploadoperation.h"


class UploadFileEntity : public QObject
{
    Q_OBJECT
public:
    explicit UploadFileEntity(QObject *parent = nullptr);
    explicit UploadFileEntity(QString filepath,QString parentid, QObject *parent = nullptr);


    QListWidgetItem *getuploaditem() {return this->uploaditem;}
    UploadListForm *getuploadlistform(){return this->uploadlistform;}
    void  start();

signals:
    void signal_uploadopeartion_begin();

public slots:



private:
    QString filepath;
    QString parentid;

    QListWidgetItem *uploaditem {uploaditem = nullptr};
    UploadListForm *uploadlistform{uploadlistform=nullptr};
    UploadOperation *uploadoperation {uploadoperation=nullptr};

    QThread * datauploadthread{datauploadthread = nullptr};




};

#endif // UPLOADFILEENTITY_H
