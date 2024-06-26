#include "localtreewidgetitem.h"

LocalTreeWidgetItem::LocalTreeWidgetItem(QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{

}

LocalTreeWidgetItem::LocalTreeWidgetItem
(QTreeWidgetItem *parent,QStringList &list):QTreeWidgetItem(parent,list){

}

LocalTreeWidgetItem::LocalTreeWidgetItem
(QStringList &list):QTreeWidgetItem(list){

}

FileMetadata *LocalTreeWidgetItem::getfilemetadata(){
    return this->filemetadata;
}

void LocalTreeWidgetItem::setfilemetadata(FileMetadata* filemetadata){
     this->filemetadata=filemetadata;
}
