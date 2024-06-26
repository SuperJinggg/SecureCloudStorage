#ifndef UPLOADLISTFORM_H
#define UPLOADLISTFORM_H

#include <QWidget>
#include <QFileInfo>
#include "../entities/filemetadata.h"

namespace Ui {
class UploadListForm;
}

class UploadListForm : public QWidget
{
    Q_OBJECT

public:
    explicit UploadListForm(QWidget *parent = nullptr);
    explicit UploadListForm(QString filepath , QWidget *parent = nullptr);
    ~UploadListForm();
    QString convert_size(qint64 size);


public slots:
    void setstat(QString);
    void setprogress(int );

private:
    Ui::UploadListForm *ui;
    QString filepath;
    FileMetadata *filemetadata{filemetadata =nullptr};
};

#endif // UPLOADLISTFORM_H
