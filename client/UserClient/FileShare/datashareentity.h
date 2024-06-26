#ifndef DATASHAREENTITY_H
#define DATASHAREENTITY_H

#include <QObject>
#include <QDebug>
#include <QtGui>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include "./datashareentity.h"
#include "global.h"
#include <QPushButton>
#include "../entities/filemetadata.h"
#include "./cancelshare.h"

class DataShareEntity : public QObject
{
    Q_OBJECT
public:
    explicit DataShareEntity(QObject *parent = nullptr);
    QString getsharedusername(){
        return this->sharedusername;
    }

    QString getshareusername(){
        return this->shareusername;
    }
    QDateTime getsharetime(){
        return this->sharetime;
    }
    QString getfilename(){
        return this->filename;
    }
    qint64 getfilesize(){
        return this->filesize;
    }

    QString getfileuniqueid(){
        return this->fileuniqueid;
    }
    bool getmark(){
        return this->mark;
    }
    QPushButton * getpushbutton(){
        return this->pushbutton;
    }



    void setsharedusername(QString sharedusername){
         this->sharedusername = sharedusername;
    }
    void setshareusername(QString shareusername){
         this->shareusername = shareusername;
    }

    void setsharetime(QDateTime sharetime){
         this->sharetime = sharetime;
    }
    void setfilename(QString filename){
         this->filename = filename;
    }
    void setfilesize(qint64 filesize){
        this->filesize = filesize;
    }
    void setmark(bool mark){
        this->mark=mark;
    }

     setpushbutton(QPushButton * pushbutton){
        this->pushbutton=pushbutton;
    }

    void setfileuniqueid(QString fileuniqueid){
        this->fileuniqueid = fileuniqueid;
    }
signals:
    void signal_cancel_result(QString);

public slots:
    void cancel_btn_clicked();
    void slot_cancel_result(QString);
    void download_btn_clicked();

private:
    QString sharedusername;
    QDateTime sharetime;
    QString filename;
    qint64 filesize;
    QString fileuniqueid;
    QString shareusername;
    bool mark; //true表示分享数据，false表示被分享的数据
    QPushButton *pushbutton { pushbutton = nullptr};
};

#endif // DATASHAREENTITY_H
