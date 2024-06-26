#include "locallogdialog.h"
#include "ui_locallogdialog.h"

QMutex LocalLogDialog::_mutex;

LocalLogDialog::LocalLogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LocalLogDialog)
{
    ui->setupUi(this);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

LocalLogDialog::~LocalLogDialog()
{
    delete ui;
}


void LocalLogDialog::relayout(QString str){
    _mutex.lock();
    if(nullptr==str){
        this->logtext = "";
    }else{
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString str_currenttime = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
        this->logtext+="\n["+str_currenttime +":] "+ str;
    }
    ui->log_textEdit->setText(this->logtext);
    //设置自动滚屏
    QTextCursor cursor = ui->log_textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->log_textEdit->setTextCursor(cursor);
    _mutex.unlock();
}
void LocalLogDialog::addlogtext(QString str){
    relayout(str);
}

void LocalLogDialog::on_clear_pushButton_clicked()
{
    relayout(nullptr);
}
