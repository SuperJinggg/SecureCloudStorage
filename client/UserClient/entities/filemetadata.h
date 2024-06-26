#ifndef FILEMETADATA_H
#define FILEMETADATA_H

#include <QObject>
#include <QWidget>
#include <QDateTime>
#include <QList>
#include <QDebug>
#include <QTableWidgetItem>
#include "../operationwidget.h"

class FileMetadata : public QObject
{
    Q_OBJECT
public:
    explicit FileMetadata(QObject *parent = nullptr);

    QString getfilename();
    long getfilesize();
    QDateTime getctime();
    QDateTime getmtime();
    QDateTime getuploadtime();
    QString getfileowner();
    QString getplaintexthash();
    QString getcipherhash();
    QString getkeyhash();
    QString getfileuniqueid();
    QString gettype();
    QString getcurrentid();
    QString getparentid();
    bool getifsheared();
    bool getifopened();
    QString getsavepath();
    QList<FileMetadata * > *getchildren();
    OperationWidget * getoperationwidget();

    QTableWidgetItem * getcheckitem();



    void setfilename(QString);
    void setfilesize(long);
    void setctime(QDateTime);
    void setmtime(QDateTime);
    void setuploadtime(QDateTime);
    void setfileowner(QString);
    void setplaintexthash(QString);
    void setcipherhash(QString);
    void setkeyhash(QString);
    void setfileuniqueid(QString);
    void settype(QString);
    void setcurrentid(QString);
    void setparentid(QString);
    void setifsheared(bool);
    void setifopened(bool ifopened);
    void setchildren(QList<FileMetadata * > *children);
    void setsavepath(QString);

    void setoperationwidget(OperationWidget *operationwidget);

    void setcheckitem(QTableWidgetItem *);
signals:
    void signal_dataopen();
    void signal_datadownload();
    void signal_datashare();
    void signal_datadelete();

public slots:
    void slot_dataopen();
    void slot_datadownload();
    void slot_datashare();
    void slot_datadelete();
private:
    QString filename;
    long filesize;
    QDateTime ctime;
    QDateTime mtime;
    QDateTime uploadtime;
    QString fileowner;
    QString plaintexthash;
    QString cipherhash;
    QString keyhash;
    QString fileuniqueid;
    QString type;
    QString currentid;
    QString parentid;
    bool ifsheared;
    bool ifopened;
    QString savepath;
    QList<FileMetadata * > *children{children = nullptr};
    OperationWidget *operationwidget {operationwidget = nullptr};
//    QObject *children{children =nullptr};

    QTableWidgetItem *checkitem{checkitem = nullptr};

};

#endif // FILEMETADATA_H
