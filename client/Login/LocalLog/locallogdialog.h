#ifndef LOCALLOGDIALOG_H
#define LOCALLOGDIALOG_H

#include <QDialog>
#include <QMutex>
#include <QDateTime>
#include <stdarg.h>
#include <QDebug>

namespace Ui {
class LocalLogDialog;
}

class LocalLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LocalLogDialog(QWidget *parent = nullptr);
    ~LocalLogDialog();
    void addlogtext(QString str);

private slots:
    void on_clear_pushButton_clicked();
    void relayout(QString str);
private:
    Ui::LocalLogDialog *ui;

    QString logtext;
    static QMutex _mutex;
};

#endif // LOCALLOGDIALOG_H
