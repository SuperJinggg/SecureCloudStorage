#include "operationwidget.h"
#include "ui_operationwidget.h"
#include "global.h"

OperationWidget::OperationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperationWidget)
{
    ui->setupUi(this);
}

OperationWidget::~OperationWidget()
{
    delete ui;
}

void OperationWidget::on_data_open_pushButton_clicked()
{
    emit signal_dataopen();
}

void OperationWidget::on_data_download_pushButton_clicked()
{

    file_path = QFileDialog::getExistingDirectory(
                this,
                "请选择保存路径",
                "./"
                );
    if(file_path.isEmpty())
    {
        return;
    }
    else
    {
        qDebug() << file_path << endl;
    }

    emit signal_datadownload();
}

void OperationWidget::on_data_share_pushButton_clicked()
{
    emit signal_datashare();
}

void OperationWidget::on_data_delete_pushButton_clicked()
{
    emit signal_datadelete();
}
