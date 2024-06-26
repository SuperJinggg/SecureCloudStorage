#ifndef OPERATIONWIDGET_H
#define OPERATIONWIDGET_H

#include <QWidget>
#include <QFileDialog>

namespace Ui {
class OperationWidget;
}

class OperationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperationWidget(QWidget *parent = nullptr);
    ~OperationWidget();

private slots:
    void on_data_open_pushButton_clicked();

    void on_data_download_pushButton_clicked();

    void on_data_share_pushButton_clicked();

    void on_data_delete_pushButton_clicked();

signals:
    void signal_dataopen();
    void signal_datadownload();
    void signal_datashare();
    void signal_datadelete();


private:
    Ui::OperationWidget *ui;
};

#endif // OPERATIONWIDGET_H
