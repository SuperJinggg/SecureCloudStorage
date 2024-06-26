#ifndef FOLDERSTRUCTURELIST_H
#define FOLDERSTRUCTURELIST_H


#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QMap>
#include <QMenu>
#include <QSequentialAnimationGroup>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QUuid>


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

#include "../entities/localtreewidgetitem.h"
#include "../entities/filemetadata.h"

class FolderStructureList : public QObject
{
    Q_OBJECT
public:
    explicit FolderStructureList(QObject *parent = nullptr);
    FolderStructureList(int current_page,int  page_size,QTreeWidget *treewidget,QObject *parent = nullptr);
    void getfolderstructure();

    QList<FileMetadata*> * parse_json_array(QJsonArray &folderstruarray );

signals:
    void itemPressed(QTreeWidgetItem*,int);

public slots:

    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);

private:
    QTreeWidget *treewidget;
    QList<FileMetadata*> *folderstructure {folderstructure=nullptr};
    void folderstructurelist(QTreeWidgetItem *top_item, QList<FileMetadata*> *folderstructure);

    QNetworkAccessManager *net_manager{ net_manager = nullptr };
    QNetworkReply *post_reply{ post_reply = nullptr };
    int current_page;
    int  page_size;




};

#endif // FOLDERSTRUCTURELIST_H
