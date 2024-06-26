#include "filestructurelist.h"
#include "global.h"
#include "../transformentity/downloadfileentity.h"
#include "../FileShare/filesharewidget.h"

FileStructureList::FileStructureList(QObject *parent)
	: QObject(parent)
{

}

FileStructureList::FileStructureList(
	QTableWidget *tablewidget,
	QLabel *page_total_num_label,
	QLabel *metadata_totalnum_label,
	int current_page,
	int page_size,
	QListWidget *downloadlist_widget,
	QObject *parent)
	: QObject(parent)
{

	this->tablewidget = tablewidget;
	this->page_total_num_label = page_total_num_label;
	this->metadata_totalnum_label = metadata_totalnum_label;
	this->current_page = current_page;
	this->page_size = page_size;
	this->downloadlist_widget = downloadlist_widget;

	if (nullptr == downloadfilelist) {
		downloadfilelist = new QMap<QString, FileMetadata *>();
	}

}

FileStructureList::FileStructureList(
	QTableWidget *tablewidget,

	QString folderuniqueid,
	int current_page,
	int page_size,
	QObject *parent)
	: QObject(parent)
{

	this->tablewidget = tablewidget;

	this->folderuniqueid = folderuniqueid;
	this->current_page = current_page;
	this->page_size = page_size;
	if (nullptr == downloadfilelist) {
		downloadfilelist = new QMap<QString, FileMetadata *>();
	}
}

void FileStructureList::getfilemetadata(QString folderuniqueid, int current_page, int page_size)
{
	this->folderuniqueid = folderuniqueid;
	this->current_page = current_page;
	this->page_size = page_size;

	if (nullptr == net_manager) {
		net_manager = new QNetworkAccessManager(this);
		connect(net_manager, SIGNAL(finished(QNetworkReply * )),
				this, SLOT(slot_replyFinished(QNetworkReply * )));
		connect(net_manager, SIGNAL(sslErrors(QNetworkReply * , QList<QSslError>)),
				this, SLOT(slot_sslErrors(QNetworkReply * , QList<QSslError>)));
		connect(net_manager, SIGNAL(authenticationRequired(QNetworkReply * , QAuthenticator * )),
				this, SLOT(slot_provideAuthenication(QNetworkReply * , QAuthenticator * )));
	}
	QNetworkRequest network_request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_2);
	network_request.setSslConfiguration(config);
	network_request.setUrl(QUrl(url + "filelist"));
	network_request.setRawHeader("sessionid", sessionid.toUtf8());
	network_request.setRawHeader("Content-Type", "application/json");
	//    QVariantList varList;
	QVariantMap var;
	var.insert("method", "filelist");
	var.insert("version", "1.0");
	var.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

	QVariantMap requestvar;
	requestvar.insert("folderuniqueid", this->folderuniqueid);
	requestvar.insert("pagenum", this->current_page);
	requestvar.insert("pagesize", this->page_size);
	var.insert("request", requestvar);
	QJsonObject obJct = QJsonObject::fromVariantMap(var);
	QJsonDocument jsonDoc(obJct);
	QByteArray json = jsonDoc.toJson();
	QString messagejsonstr(json);
	//    messagejsonstr = QtJson::serialize(messagejsonobj);
	qDebug() << Q_FUNC_INFO << "messagejsonstr is " << messagejsonstr;
	post_reply = net_manager->post(network_request, messagejsonstr.toUtf8());
	connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
			this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));
}

void FileStructureList::slot_replyFinished(QNetworkReply *reply)
{
	QByteArray ret_data;
	QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (statusCode.isValid())
		qDebug() << Q_FUNC_INFO << "status code=" << statusCode.toInt();
	QString method = "";
	QString result = "";
	QString code = "";

	QString message = "";
	QVariantMap details;
	bool ifexist;
	if (nullptr != reply) {
		ret_data = reply->readAll();
//        qDebug() <<Q_FUNC_INFO<< "ret_data =" << ret_data;
		QString retmethod;
		QString retresult;
		QString retmessage;
		int retcode;
		QString retrole;
		QString retusername;
		QString retsessionid;
		QString retsessionkey;
		QString retlastauthtime;
		QString retlastauthaddress;

		//解析json
		QByteArray jsonbytearray;
		jsonbytearray.append(ret_data);

		QJsonParseError json_error;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonbytearray, &json_error));

		if (json_error.error != QJsonParseError::NoError) {
			qDebug() << Q_FUNC_INFO << "json error!";
			return;
		}
		QJsonObject rootObj = jsonDoc.object();
		if (rootObj.contains("method")) {
			retmethod = rootObj.value("method").toString();
		}
		if (rootObj.contains("result")) {
			retresult = rootObj.value("result").toString();
		}
		if (rootObj.contains("code")) {
			retcode = rootObj.value("code").toInt();
		}
		if (rootObj.contains("message")) {
			retmessage = rootObj.value("message").toString();
		}

		QJsonObject detailsobj;
		if (rootObj.contains("details")) {
			detailsobj = rootObj.value("details").toObject();
		}
		int filetotalnum;
		if (detailsobj.contains("filenumber")) {
			filetotalnum = detailsobj.value("filenumber").toInt();
		}
		QJsonArray filelistarray;
		if (detailsobj.contains("filelist")) {
			filelistarray = detailsobj.value("filelist").toArray();
			filelist = parse_json_array(filelistarray);
//            qDebug()<<Q_FUNC_INFO<<"filelist size is "<<filelist->size();

			this->tablewidget->setRowCount(0);
			this->tablewidget->setRowCount(filelist->size());

			QListIterator<FileMetadata *> iterater(*filelist);
			int addrow = 0;
			while (iterater.hasNext()) {
				FileMetadata *entity = iterater.next();
				QTableWidgetItem *check = new QTableWidgetItem();
				check->setCheckState(Qt::Unchecked);
				entity->setcheckitem(check);
				this->tablewidget->setItem(addrow, 0, check);
				OperationWidget *operationwidget = new OperationWidget();
				entity->setoperationwidget(operationwidget);
				this->tablewidget->setCellWidget(addrow, 1, entity->getoperationwidget());

				QTableWidgetItem *filename_item = new QTableWidgetItem();
				filename_item->setText(entity->getfilename());
				this->tablewidget->setItem(addrow, 2, filename_item);

				QTableWidgetItem *filesize_item = new QTableWidgetItem();
				filesize_item->setText(convert_size(entity->getfilesize()));
				this->tablewidget->setItem(addrow, 3, filesize_item);

				QTableWidgetItem *uploadtime_item = new QTableWidgetItem();
//                qDebug()<<Q_FUNC_INFO<<"uploadtime is "<<entity->getuploadtime();
				uploadtime_item->setText(entity->getuploadtime().toString("yyyy-MM-dd HH:mm:ss"));
				this->tablewidget->setItem(addrow, 4, uploadtime_item);

				QTableWidgetItem *type_item = new QTableWidgetItem();
				if (0 == entity->gettype().compare("dir")) {
					type_item->setText("文件夹");
				}
				else {
					type_item->setText("文件");
				}
				this->tablewidget->setItem(addrow, 5, type_item);
				connect(entity, SIGNAL(signal_dataopen()),
						this, SLOT(slot_dataopen()));
				connect(entity, SIGNAL(signal_datadownload()),
						this, SLOT(slot_datadownload()));
				connect(entity, SIGNAL(signal_datashare()),
						this, SLOT(slot_datashare()));
				connect(entity, SIGNAL(signal_datadelete()),
						this, SLOT(slot_datadelete()));
				addrow++;
			}
			this->tablewidget->sortItems(4);
			this->page_total_num_label
				->setText("共" + QString::number(((int)(filelist->size() / page_size)) + 1) + "页");
			this->metadata_totalnum_label->setText("显示" + QString::number((current_page * page_size) + 1)
													   + "-"
													   + QString::number(filelist->size() - (current_page) * page_size)
													   + "，共" + QString::number(filelist->size()) + "条记录");
		}
	}
}

void FileStructureList::slot_dataopen()
{
	FileMetadata *entity = (FileMetadata *)sender();
	qDebug() << Q_FUNC_INFO << "entity name is " << entity->getfilename();
	qDebug() << Q_FUNC_INFO << "entity getfileuniqueid is " << entity->getfileuniqueid();
}

void FileStructureList::slot_datadownload()
{
	FileMetadata *entity = (FileMetadata *)sender();
	qDebug() << Q_FUNC_INFO << "entity name is " << entity->getfilename();
	qDebug() << Q_FUNC_INFO << "entity getfileuniqueid is " << entity->getfileuniqueid();
	downloadfilelist->insert(entity->getfileuniqueid(), entity);
	if (downloadfilelist->size() > 0) {
		refreshdownloadlistview();
	}
}

void FileStructureList::slot_datashare()
{
	FileMetadata *entity = (FileMetadata *)sender();
	qDebug() << Q_FUNC_INFO << "entity name is " << entity->getfilename();
	qDebug() << Q_FUNC_INFO << "entity getfileuniqueid is " << entity->getfileuniqueid();

	FileShareWidget *fileshare = new FileShareWidget(entity);
//    connect(fileshare, SIGNAL(file_share_signal(FileMetadata* )),
//            this, SLOT(slot_file_share(FileMetadata* )));
	//模态对话框,点击弹出子对话框，然后父窗口不可点击
	fileshare->setWindowModality(Qt::ApplicationModal);
	fileshare->show();
}

//void  FileStructureList::slot_file_share(FileMetadata* filemetadata){
//    qDebug()<<Q_FUNC_INFO<<"in slot_file_share"<<filemetadata->getfilename();
//}
void FileStructureList::slot_datadelete()
{
	FileMetadata *entity = (FileMetadata *)sender();
	qDebug() << Q_FUNC_INFO << "entity name is " << entity->getfilename();
	qDebug() << Q_FUNC_INFO << "entity getfileuniqueid is " << entity->getfileuniqueid();

	QNetworkRequest network_request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_2);
	network_request.setSslConfiguration(config);
	network_request.setUrl(QUrl(url + "datadelete"));

	network_request.setRawHeader("sessionid", sessionid.toUtf8());
	network_request.setRawHeader("Content-Type", "application/json");
	QVariantMap messagejsonvar;
	messagejsonvar.insert("method", "datadelete");
	messagejsonvar.insert("version", "1.0");
	messagejsonvar.insert("timestamp", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	QVariantMap requestvar;
	requestvar.insert("fileuniqueid", entity->getfileuniqueid());
	messagejsonvar.insert("request", requestvar);
	QJsonObject obJct = QJsonObject::fromVariantMap(messagejsonvar);
	QJsonDocument jsonDoc(obJct);
	QByteArray json = jsonDoc.toJson();
	QString messagejsonstr(json);
	qDebug() << Q_FUNC_INFO << "messagejsonstr is " << messagejsonstr;
	post_reply = net_manager->post(network_request, messagejsonstr.toUtf8());
	connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
			this, SLOT(slot_NetWorkError(QNetworkReply::NetworkError)));

	downloadfilelist->remove(entity->getfileuniqueid());
	if (downloadfilelist->size() > 0) {
		refreshdownloadlistview();
	}
	emit filelist_refresh();
}

void FileStructureList::batch_download()
{
	if (nullptr != filelist) {
		QListIterator<FileMetadata *> iterater(*filelist);
		while (iterater.hasNext()) {
			FileMetadata *entity = iterater.next();
			QTableWidgetItem *checkitem = entity->getcheckitem();
			if (Qt::Checked == checkitem->checkState()) {
				downloadfilelist->insert(entity->getfileuniqueid(), entity);
			}
		}
		refreshdownloadlistview();
	}
}

void FileStructureList::refreshdownloadlistview()
{
	qDebug() << Q_FUNC_INFO << "in refreshdownloadlistview";
	if (nullptr != downloadfilelist && downloadfilelist->size() > 0) {
		QMap<QString, FileMetadata *>::const_iterator iterater = downloadfilelist->constBegin();
		while (iterater != downloadfilelist->constEnd()) {
			qDebug() << Q_FUNC_INFO << "in while";
			QString downloadfilelistkey = iterater.key();
			FileMetadata *downloadfilemetadata = iterater.value();
			DownloadFileEntity *downloadfileentity = new DownloadFileEntity(downloadfilemetadata);
			QListWidgetItem *downloaditem = downloadfileentity->getdownloaditem();
			downloaditem->setSizeHint(QSize(downloaditem->sizeHint().width(), 60));
			DownloadListForm *uploadlistform = downloadfileentity->getdownloadlistform();
			this->downloadlist_widget->addItem(downloaditem);
			this->downloadlist_widget->setItemWidget(downloaditem, uploadlistform);
			downloadfileentity->start();
			iterater++;
		}
		downloadfilelist->clear();
	}
}

QList<FileMetadata *> *FileStructureList::parse_json_array(QJsonArray &filelistarray)
{

	QList<FileMetadata *> *childrenfolderstructure = new QList<FileMetadata *>();
	for (int i = 0; i < filelistarray.size(); i++) {
		FileMetadata *folderstruc = new FileMetadata();
		QJsonObject pobj = filelistarray.at(i).toObject();
		int id;
		QString filename;
		int filesize;
		QString ctime;
		QString mtime;
		QString lastaccesstime;
		QString uploadtime;
		QString plaintexthash;
		QString ciphertexthash;
		int ciphertextsize;
		QString keyhash;
		QString fileuniqueid;
		QString type;
		QString currentid;
		QString parentid;
		bool ifshared;
		bool ifopened;

		qDebug()<<Q_FUNC_INFO<<"pobj is "<<pobj;

		if (pobj.contains("id")) {
			id = pobj.value("id").toInt();
		}
		if (pobj.contains("filename")) {
			filename = pobj.value("filename").toString();
			folderstruc->setfilename(filename);
		}
		if (pobj.contains("filesize")) {
			filesize = pobj.value("filesize").toInt();
			folderstruc->setfilesize(filesize);
		}
		if (pobj.contains("ctime")) {
			ctime = pobj.value("ctime").toString();
			QDateTime ctimedata = QDateTime::fromString(ctime, "yyyy-MM-dd HH:mm:ss");
			folderstruc->setctime(ctimedata);
		}
		if (pobj.contains("mtime")) {
			mtime = pobj.value("mtime").toString();
			QDateTime mtimedata = QDateTime::fromString(mtime, "yyyy-MM-dd HH:mm:ss");
			folderstruc->setmtime(mtimedata);
		}
		if (pobj.contains("uploadtime")) {
			uploadtime = pobj.value("uploadtime").toString();
			QDateTime uploadtimedata;

			uploadtimedata = QDateTime::fromString(uploadtime, "yyyy-MM-ddTHH:mm:ss");
//            qDebug()<<Q_FUNC_INFO<<"uploadtimedata is "<<uploadtimedata;
			folderstruc->setuploadtime(uploadtimedata);
		}
		if (pobj.contains("plaintexthash")) {
			plaintexthash = pobj.value("plaintexthash").toString();
			folderstruc->setplaintexthash(plaintexthash);
		}
		if (pobj.contains("ciphertexthash")) {
			ciphertexthash = pobj.value("ciphertexthash").toString();
			folderstruc->setcipherhash(ciphertexthash);
		}
		if (pobj.contains("keyhash")) {
			keyhash = pobj.value("keyhash").toString();
			folderstruc->setkeyhash(keyhash);
		}
		if (pobj.contains("fileuniqueid")) {
			fileuniqueid = pobj.value("fileuniqueid").toString();
			folderstruc->setfileuniqueid(fileuniqueid);
		}
		if (pobj.contains("type")) {
			type = pobj.value("type").toString();
			folderstruc->settype(type);
		}
		if (pobj.contains("currentid")) {
			currentid = pobj.value("currentid").toString();
			folderstruc->setcurrentid(currentid);
		}
		if (pobj.contains("parentid")) {
			parentid = pobj.value("parentid").toString();
			folderstruc->setparentid(parentid);
		}
		if (pobj.contains("ifshared")) {
			ifshared = pobj.value("ifshared").toBool();
			folderstruc->setifsheared(ifshared);
		}
		if (pobj.contains("ifopened")) {
			ifopened = pobj.value("ifopened").toBool();
			folderstruc->setifopened(ifopened);
		}

		if (pobj.contains("currentid")) {
			currentid = pobj.value("currentid").toString();
			folderstruc->setcurrentid(currentid);
		}
		if (pobj.contains("opened")) {
			ifopened = pobj.value("opened").toBool();
			folderstruc->setifopened(ifopened);
		}
		childrenfolderstructure->append(folderstruc);
	}
	return childrenfolderstructure;
}

void FileStructureList::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	QSslCertificate sslcert = errors[0].certificate();
	reply->ignoreSslErrors();
}

void FileStructureList::slot_provideAuthenication(QNetworkReply *reply, QAuthenticator *authenticator)
{

}
QString FileStructureList::convert_size(qint64 size)
{
	QString prefix;
	double speed = size;
	if (speed <= 900) {
		prefix = "B";
	}

	if (speed > 900) {
		speed = speed / 1024;
		prefix = "K";
	}
	if (speed > 900) {
		speed = speed / 1024;
		prefix = "M";
	}
	if (speed > 900) {
		speed = speed / 1024;
		prefix = "G";
	}
	if (speed > 900) {
		speed = speed / 1024;
		prefix = "T";
	}
	if (speed > 900) {
		speed = speed / 1024;
		prefix = "P";
	}


	QString data = QString::number(speed);

	return data + prefix;

}
void FileStructureList::slot_NetWorkError(QNetworkReply::NetworkError)
{
	if (nullptr != post_reply) {
		post_reply->deleteLater();
		post_reply = nullptr;
	}
	if (nullptr != net_manager) {
		delete net_manager;
		net_manager = nullptr;
	}
}
