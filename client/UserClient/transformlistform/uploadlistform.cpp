#include "uploadlistform.h"
#include "ui_uploadlistform.h"

UploadListForm::UploadListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UploadListForm)
{
    ui->setupUi(this);
    //    ui->name_label->setText("");
}


UploadListForm::UploadListForm(QString filepath , QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UploadListForm)
{
    ui->setupUi(this);
    this->filepath=filepath;
    QFileInfo file(this->filepath);
    ui->name_label->setText(file.fileName());
    ui->size_label->setText(convert_size(file.size()));
}

void UploadListForm::setstat(QString stat){
    ui->state_label->setText(stat);
}

void UploadListForm::setprogress(int progress){
    ui->progressBar->setValue(progress);
}


QString UploadListForm::convert_size(qint64 size){
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

UploadListForm::~UploadListForm()
{
    delete ui;
}
