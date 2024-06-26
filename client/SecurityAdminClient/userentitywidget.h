#ifndef USERENTITYWIDGET_H
#define USERENTITYWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class UserEntityWidget;
}

class UserEntityWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserEntityWidget(QWidget *parent = nullptr);
    ~UserEntityWidget();



public:
    int getid();
    QString getusername();
    QString getrole();
    QString getstate();
    QString getregistertime();
    QString getforbid_time();
    QTableWidgetItem *getqcheckbox();


    void setid(int);
    void setusername(QString);
    void setrole(QString);
    void setstate(QString);
    void setregistertime(QString);
    void setforbid_time(QString);
    void setqcheckbox(QTableWidgetItem * );
public slots:
    void slot_replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slot_provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void slot_NetWorkError(QNetworkReply::NetworkError);
private slots:
    void on_operationwidget_delete_pushButton_clicked();

private:
    Ui::UserEntityWidget *ui;
    int id;
    QString username;
    QString role;
    QString state;
    QString registertime;
    QString forbid_time;

    QNetworkReply *post_reply{ post_reply = nullptr };
    QTableWidgetItem *qcheckbox{qcheckbox=nullptr};
    QNetworkAccessManager *manager{ manager = nullptr };

    void sendrequest(QString username);

signals:
    void refresh(QString role);


};

#endif // USERENTITYWIDGET_H
