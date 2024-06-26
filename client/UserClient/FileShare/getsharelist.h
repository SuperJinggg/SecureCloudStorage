#ifndef GETSHARELIST_H
#define GETSHARELIST_H

#include <QObject>
#include <QUuid>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QProcess>
#include <QListWidgetItem>
#include <QThread>
#include <QDebug>
#include <QList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonParseError>
#include "global.h"
#include "../entities/filemetadata.h"
#include "./datashareentity.h"


class GetShareList : public QObject
{
    Q_OBJECT
public:
    explicit GetShareList(QObject *parent = nullptr);
    explicit GetShareList(QTableWidget * tablewidget,QObject *parent = nullptr);

    void get_data();

    void refreshsharelistview(QList<DataShareEntity*> *);
    QList<DataShareEntity*> * parse_json_array(QJsonArray &filelistarray );
signals:

public slots:
    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);
//    void cancel_btn_clicked();
    void slot_cancel_result(QString);
private:
    QTableWidget * tablewidget {tablewidget = nullptr};
    QNetworkAccessManager *net_manager{ net_manager = nullptr };
    QNetworkReply *post_reply{ post_reply = nullptr };
//    QList<DataShareEntity*> * sharelist {sharelist = nullptr};
    QString convert_size(qint64 size);
};

#endif // GETSHARELIST_H
