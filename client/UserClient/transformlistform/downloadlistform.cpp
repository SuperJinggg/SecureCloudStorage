#include "downloadlistform.h"
#include "ui_downloadlistform.h"

DownloadListForm::DownloadListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadListForm)
{
    ui->setupUi(this);
}
DownloadListForm::DownloadListForm(FileMetadata* downloadfilemetadata,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadListForm)
{
    this->filemetadata=downloadfilemetadata;
    ui->setupUi(this);
    ui->name_label->setText(this->filemetadata->getfilename());
    ui->size_label->setText(convert_size(this->filemetadata->getfilesize()));
    ui->state_label->setText("正在下载");
}



DownloadListForm::~DownloadListForm()
{
    delete ui;
}


void DownloadListForm::setstat(QString){

}

void DownloadListForm::setprogress(int porgress){
    ui->progressBar->setValue(porgress);
}
QString DownloadListForm::convert_size(qint64 size){
    QString prefix;
    double speed = size;
    if(speed<=900){
        prefix = "B";
    }

    if(speed>900){
        speed = speed/1024;
        prefix = "K";
    }
    if(speed>900){
        speed = speed/1024;
        prefix = "M";
    }
    if(speed>900){
        speed = speed/1024;
        prefix = "G";
    }
    if(speed>900){
        speed = speed/1024;
        prefix = "T";
    }
    if(speed>900){
        speed = speed/1024;
        prefix = "P";
    }


    QString data = QString::number(speed);

    return data+prefix;

}

void DownloadListForm::on_decrypt_open_pushButton_clicked()
{
    emit signal_decrypt_and_open_file();
}
