#ifndef SECCLOUDLOG_H
#define SECCLOUDLOG_H

#include <QObject>
#include <QDebug>
#include "../global.h"

class SecCloudLog : public QObject
{
    Q_OBJECT
public:
    explicit SecCloudLog(QObject *parent = nullptr);
    void static PrintLog(QString,QString,int);
signals:

public slots:
};

#endif // SECCLOUDLOG_H
