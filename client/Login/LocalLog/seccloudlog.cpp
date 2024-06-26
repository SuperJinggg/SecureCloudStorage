#include "seccloudlog.h"

SecCloudLog::SecCloudLog(QObject *parent) : QObject(parent)
{

}
void SecCloudLog::PrintLog(QString funcinfo ,QString message,int level) {
    if(level>=loglevel){
        if (nullptr != locallogdialog&&locallogdialog->isVisible()) {
            locallogdialog->addlogtext(funcinfo + message);
        }else{
            qDebug() << funcinfo << message;
        }
    }
    return;
}
