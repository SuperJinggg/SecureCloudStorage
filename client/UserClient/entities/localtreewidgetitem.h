#ifndef LOCALTREEWIDGETITEM_H
#define LOCALTREEWIDGETITEM_H

#include <QObject>
#include <QTreeWidgetItem>
#include "./filemetadata.h"

class LocalTreeWidgetItem : public QObject,public QTreeWidgetItem
{
    Q_OBJECT
public:
    LocalTreeWidgetItem(QTreeWidgetItem *parent);
    LocalTreeWidgetItem(QTreeWidgetItem *parent,QStringList &list);
    LocalTreeWidgetItem(QStringList &list);

    FileMetadata *getfilemetadata();
    void setfilemetadata(FileMetadata*);



signals:

public slots:

private:

    FileMetadata *filemetadata{filemetadata = nullptr};


};

#endif // LOCALTREEWIDGETITEM_H
