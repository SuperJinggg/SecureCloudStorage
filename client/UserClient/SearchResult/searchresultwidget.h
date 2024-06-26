#ifndef SEARCHRESULTWIDGET_H
#define SEARCHRESULTWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>
#include <QLabel>
#include <QMenu>
#include <QSequentialAnimationGroup>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QUuid>
#include <QProcess>
#include <QListWidgetItem>
#include <QThread>
#include <QDateTime>
#include <QList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "global.h"
#include "../local_lineedit/local_lineedit.h"
#include "../lib/include/cipherinterface.h"
#include "../entities/filemetadata.h"

namespace Ui {
class SearchResultWidget;
}

class SearchResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchResultWidget(QWidget *parent = 0);

    void data_search(
            QTabWidget *tabwidget,
            QString keywords,
            QString type,
            QString mode,
            QDateTime fromtime,
            QDateTime totime);


    QList<FileMetadata*> * parse_json_array(QJsonArray &filelistarray );
    QString convert_size(qint64 size);
    ~SearchResultWidget();


private slots:
    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);

    void slot_jumptopage_linedit_focussed(bool);
private:
    Ui::SearchResultWidget *ui;
    QTabWidget *tabwidget;
    QString keywords;
    QString type;
    QString mode;
    QDateTime fromtime;
    QDateTime totime;

    QNetworkAccessManager *net_manager{ net_manager = nullptr };
    QNetworkReply *post_reply{ post_reply = nullptr };

    QList<FileMetadata*> * filelist{filelist=nullptr};

    Local_LineEdit *local_linedit {local_linedit = nullptr};
    int current_page = 0;
    int page_size = 0;
};

#endif // SEARCHRESULTWIDGET_H
