#ifndef DOWNLOADLISTFORM_H
#define DOWNLOADLISTFORM_H

#include <QWidget>
#include "../entities/filemetadata.h"

namespace Ui {
class DownloadListForm;
}

class DownloadListForm : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadListForm(QWidget *parent = nullptr);

    DownloadListForm(FileMetadata* downloadfilemetadata,QWidget *parent = nullptr);

    ~DownloadListForm();

private slots:
    void setstat(QString);
    void setprogress(int);
    void on_decrypt_open_pushButton_clicked();

signals:
    void signal_decrypt_and_open_file();

private:
    Ui::DownloadListForm *ui;

    FileMetadata *filemetadata{filemetadata =nullptr};

    QString convert_size(qint64 size);
};

#endif // DOWNLOADLISTFORM_H
