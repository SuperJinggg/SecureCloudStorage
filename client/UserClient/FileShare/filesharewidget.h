#ifndef FILESHAREWIDGET_H
#define FILESHAREWIDGET_H

#include <QWidget>
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
namespace Ui {
class FileShareWidget;
}

class FileShareWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileShareWidget(QWidget *parent = nullptr);
    explicit FileShareWidget(FileMetadata* entity,QWidget *parent = nullptr);
    ~FileShareWidget();

signals:
//    void file_share_signal(FileMetadata* entity);

private slots:
    void on_confirm_pushButton_clicked();

    void on_cancel_pushButton_clicked();


    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);

private:
    Ui::FileShareWidget *ui;
    FileMetadata* entity{entity=nullptr};

    QNetworkAccessManager *net_manager{ net_manager = nullptr };
    QNetworkReply *post_reply{ post_reply = nullptr };

};

#endif // FILESHAREWIDGET_H
