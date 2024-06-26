#include "datashareentity.h"
#include "global.h"
#include "transformentity/downloadfileentity.h"
#include <winsock.h>
#include "../FileStructureList/filestructurelist.h"

DataShareEntity::DataShareEntity(QObject *parent)
	: QObject(parent)
{

}

void DataShareEntity::cancel_btn_clicked()
{
	CancelShare *cancelshare = new CancelShare();
	connect(cancelshare, SIGNAL(signal_cancel_result(QString)), this, SLOT(slot_cancel_result(QString)));
	cancelshare->cancel_share(this->fileuniqueid, this->sharedusername);
}

void DataShareEntity::slot_cancel_result(QString result)
{
	emit signal_cancel_result(result);
}

void DataShareEntity::download_btn_clicked()
{

}
