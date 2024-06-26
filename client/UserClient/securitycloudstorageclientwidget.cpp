#include "securitycloudstorageclientwidget.h"
#include "ui_securitycloudstorageclientwidget.h"

SecurityCloudStorageClientWidget::SecurityCloudStorageClientWidget(QWidget *parent)
	:
	QWidget(parent),
	ui(new Ui::SecurityCloudStorageClientWidget)
{
	ui->setupUi(this);
	ui->notice_label->setVisible(false);
	ui->splitter->setStretchFactor(0, 3);
	ui->splitter->setStretchFactor(1, 7);
	ui->data_list_tableWidget->setSortingEnabled(true);
	ui->data_list_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui->data_list_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	ui->data_list_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
	ui->data_list_tableWidget->setRowCount(0);
	ui->data_list_tableWidget->setColumnWidth(0, 40);  //设置列宽
	ui->data_list_tableWidget->setColumnWidth(1, 130);


	ui->ishare_tableWidget->setSortingEnabled(true);
	ui->ishare_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui->ishare_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	ui->ishare_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
	ui->ishare_tableWidget->setRowCount(0);
	ui->ishare_tableWidget->setColumnWidth(0, 150);  //设置列宽
	ui->ishare_tableWidget->setColumnWidth(1, 230);
	ui->ishare_tableWidget->setColumnWidth(2, 230);
	ui->ishare_tableWidget->setColumnWidth(3, 230);
	//    ui->ishare_tableWidget->setColumnWidth(4, 230);

	ui->shared_tome_tableWidget->setSortingEnabled(true);
	ui->shared_tome_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui->shared_tome_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	ui->shared_tome_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
	ui->shared_tome_tableWidget->setRowCount(0);
	ui->shared_tome_tableWidget->setColumnWidth(0, 150);  //设置列宽
	ui->shared_tome_tableWidget->setColumnWidth(1, 230);
	ui->shared_tome_tableWidget->setColumnWidth(2, 230);
	ui->shared_tome_tableWidget->setColumnWidth(3, 230);
	//    ui->shared_tome_tableWidget->setColumnWidth(4, 230);


	ui->lastlogintime_label->setText(lashauthtime);
	ui->lastauth_ipaddress_label->setText(lastauthipaddress);
	ui->username_label->setText(username);

	if (nullptr == local_linedit) {
		local_linedit = new Local_LineEdit();
		local_linedit->setValidator(new QIntValidator(1, 1000, this)); //1, 1000为输入的数字值范围
		connect(local_linedit,
				SIGNAL(signal_linedit_focussed(bool)),
				this, SLOT(slot_jumptopage_linedit_focussed(bool)));
		local_linedit->setText(QString::number(current_page + 1));
		ui->jumptopage_lineEdit_horizontalLayout->addWidget(local_linedit);
	}
	ui->filelist_tabWidget->setTabsClosable(true);
	ui->filelist_tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);


	if (nullptr == searchpage_search_linedit) {
		searchpage_search_linedit = new Local_LineEdit();
		searchpage_search_linedit->setMinimumHeight(40);
		searchpage_search_linedit->setMinimumWidth(539);
		searchpage_search_linedit
			->setStyleSheet("QLineEdit:{font: 12pt \"微软雅黑\"; min-height: 26px;min-height: 270px}");
		searchpage_search_linedit->setPlaceholderText("多个词之间以“&”或“|”间隔，表示“与”或“或”的关系");
		searchpage_search_linedit->setToolTip("多个词之间以“&”或“|”间隔，表示“与”或“或”的关系");
		connect(searchpage_search_linedit,
				SIGNAL(signal_linedit_focussed(bool)),
				this, SLOT(slot_searchpage_search_linedit_focussed(bool)));
		ui->main_search_horizontalLayout->addWidget(searchpage_search_linedit);
	}

	ui->advanced_from_dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-7));
	ui->advanced_to_dateTimeEdit->setDateTime(QDateTime::currentDateTime());


	QMenu *select_menu = new QMenu();
	QAction *name_search = new QAction("文件名检索");
	QAction *cipher_search = new QAction("密文检索");
	select_menu->addAction(name_search);
	select_menu->addAction(cipher_search);

	connect(name_search, SIGNAL(triggered()), this, SLOT(slot_namesearch_clicked()));
	connect(cipher_search, SIGNAL(triggered()), this, SLOT(slot_ciphersearch_clicked()));


	//    connect(ui->share_tabWidget,
	//                SIGNAL(tabBarClicked(int )),
	//                this,SLOT(slot_datashare_tabwidget_clicked(int)));


	connect(ui->share_tabWidget,
			SIGNAL(tabBarClicked(int)),
			this, SLOT(slot_datashare_tabwidget_clicked(int)));

	ui->select_pushButton->setMenu(select_menu);


	QMenu *search_tabWidget_select_menu = new QMenu();
	QAction *search_tabWidget_name_search = new QAction("文件名检索");
	QAction *search_tabWidget_cipher_search = new QAction("密文检索");
	search_tabWidget_select_menu->addAction(search_tabWidget_name_search);
	search_tabWidget_select_menu->addAction(search_tabWidget_cipher_search);
	connect(search_tabWidget_name_search, SIGNAL(triggered()), this, SLOT(slot_search_tabWidget_namesearch_clicked()));
	connect(search_tabWidget_cipher_search,
			SIGNAL(triggered()),
			this,
			SLOT(slot_search_tabWidget_ciphersearch_clicked()));
	ui->search_select_pushButton->setMenu(search_tabWidget_select_menu);


	ui->search_tabWidget->setTabsClosable(true);
	ui->search_tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);

	connect(ui->search_tabWidget,
			SIGNAL(tabCloseRequested(int)),
			this, SLOT(slot_search_result_tabwidget_closed(int)));


	//    QPixmap pm(35, 35);
	QPixmap pm(80, 80);
	pm.fill(Qt::transparent);
	QPainter painter(&pm);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen pen(Qt::black, 1);
	painter.setPen(pen);
	QBrush brush(Qt::red);
	painter.setBrush(brush);
	painter.drawEllipse(0, 0, 80, 80);
	QFont font;
	font.setFamily("Microsoft YaHei");
	font.setPointSize(50);
	font.setBold(true);
	painter.setFont(font);
	painter.drawText(QRect(0, 0, 80, 80), Qt::AlignCenter, QString::number(2));
	QIcon myqicon(pm);
	page_size = ui->show_num_comboBox->currentText().toInt();
	if (nullptr == folderstrulist) {
		folderstrulist = new FolderStructureList(current_page, page_size, ui->folder_treeWidget);
		connect(ui->folder_treeWidget, SIGNAL(itemPressed(QTreeWidgetItem * , int)),
				this, SLOT(TreeWidgetItemPressed_Slot(QTreeWidgetItem * , int)));
		connect(folderstrulist, SIGNAL(itemPressed(QTreeWidgetItem * , int)),
				this, SLOT(TreeWidgetItemPressed_Slot(QTreeWidgetItem * , int)));
	}


	if (nullptr == filestrulist) {
		filestrulist = new FileStructureList(
			ui->data_list_tableWidget,
			ui->page_total_num_label,
			ui->metadata_totalnum_label,
			this->current_page,
			this->page_size,
			ui->downloadlist_listWidget);

		connect(filestrulist,SIGNAL(filelist_refresh()),this,SLOT(on_refresh_pushButton_clicked()));
	}


	ui->normal_search_widget->setVisible(true);
	ui->advanced_search_widget->setVisible(false);
	if (nullptr == uploadentityhash) {
		uploadentityhash = new QMap<QString, UploadFileEntity *>;
	}
}

void SecurityCloudStorageClientWidget::slot_datashare_tabwidget_clicked(int index)
{
#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_
	QString logmessage;
	QTextStream logmessageout(&logmessage);
	logmessageout << "index is " << index;
	SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage, 1);
#endif // _SKLOIS_CONTENTSCROOL_DEBUG_
	if (0 == index) {
		GetShareList *getsharelist = new GetShareList(ui->ishare_tableWidget, 0);
		getsharelist->get_data();
	}
	else if (1 == index) {
		GetSharedList *getsharedlist = new GetSharedList(ui->shared_tome_tableWidget, 0);
		getsharedlist->get_data();
	}
}

void SecurityCloudStorageClientWidget::TreeWidgetItemPressed_Slot(QTreeWidgetItem *pressedItem, int column)
{

	qDebug() << Q_FUNC_INFO << "in TreeWidgetItemPressed_Slot";

	auto localpressedItem = dynamic_cast<LocalTreeWidgetItem *>(pressedItem);
	if (qApp->mouseButtons() == Qt::RightButton) {
		qDebug() << Q_FUNC_INFO << "TreeWidgetItemPressed_Slot";
		QMenu *pMenu = new QMenu(this);
		pMenu->addAction(QString("新建文件夹"));
		pMenu->addSeparator();
		pMenu->addAction(QString("重命名"));
		pMenu->exec(QCursor::pos());
	}
	else if (qApp->mouseButtons() == Qt::LeftButton) {
		if (nullptr != localpressedItem->getfilemetadata()) {
			qDebug() << Q_FUNC_INFO << localpressedItem->getfilemetadata()->getcurrentid();
			if (nullptr != filestrulist) {
				current_selected_folder_uniqueid = localpressedItem->getfilemetadata()->getcurrentid();
				filestrulist->getfilemetadata(
					localpressedItem->getfilemetadata()->getcurrentid(),
					this->current_page,
					this->page_size);
			}
		}
		else {
			qDebug() << Q_FUNC_INFO << "根目录";
			if (nullptr != filestrulist) {
				current_selected_folder_uniqueid = "根目录";
				filestrulist->getfilemetadata(
					"根目录",
					this->current_page,
					this->page_size);
			}
		}
	}
	else {
		current_selected_folder_uniqueid = "根目录";
		qDebug() << Q_FUNC_INFO << "根目录";
		if (nullptr != filestrulist) {
			filestrulist->getfilemetadata(
				"根目录",
				this->current_page,
				this->page_size);
		}
	}
}

void SecurityCloudStorageClientWidget::slot_jumptopage_linedit_focussed(bool stat)
{

}

void SecurityCloudStorageClientWidget::slot_searchpage_search_linedit_focussed(bool stat)
{

}

void SecurityCloudStorageClientWidget::slot_namesearch_clicked()
{

#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_
	//这个是日志打印。
	QString logmessage;
	QTextStream logmessageout(&logmessage);
	logmessageout << "in slot_namesearch_clicked";;
	SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage, 1);
#endif

	ui->search_pushButton->setText("文件名检索");
}

void SecurityCloudStorageClientWidget::slot_ciphersearch_clicked()
{
	//    qDebug()<<Q_FUNC_INFO<<"in slot_ciphersearch_clicked";
	//这个是日志打印。
#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_
	QString logmessage;
	QTextStream logmessageout(&logmessage);
	logmessageout << "in slot_ciphersearch_clicked";;
	SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage, 1);
#endif
	ui->search_pushButton->setText("密文检索");
}

void SecurityCloudStorageClientWidget::slot_search_tabWidget_namesearch_clicked()
{
	ui->search_tabWidget_search_pushButton->setText("文件名检索");
}

void SecurityCloudStorageClientWidget::slot_search_tabWidget_ciphersearch_clicked()
{
	ui->search_tabWidget_search_pushButton->setText("密文检索");
}

SecurityCloudStorageClientWidget::~SecurityCloudStorageClientWidget()
{
	if (nullptr != locallogdialog) {
		locallogdialog->close();
	}
	delete ui;
}

void SecurityCloudStorageClientWidget::on_document_management_pushButton_clicked()
{
	ui->main_stackedWidget->setCurrentIndex(0);

	ui->document_management_pushButton->
		setStyleSheet(
		"QPushButton#document_management_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/shuju_en.png);"
		"}"
		"QPushButton#document_management_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/shuju_hua.png);"
		"}"
		"QPushButton#document_management_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/shuju_en.png);"
		"}"
	);
	ui->document_search_pushButton->
		setStyleSheet(
		"QPushButton#document_search_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_mo.png);"
		"}"
		"QPushButton#document_search_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_hua.png);"
		"}"
		"QPushButton#document_search_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_en.png);"
		"}"
	);
	ui->transport_list_pushButton->
		setStyleSheet(
		"QPushButton#transport_list_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_mo.png);"
		"}"
		"QPushButton#transport_list_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_hua.png);"
		"}"
		"QPushButton#transport_list_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_en.png);"
		"}"
	);

	ui->share_list_pushButton->
		setStyleSheet(
		"QPushButton#share_list_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_mo.png);"
		"}"
		"QPushButton#share_list_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_hua.png);"
		"}"
		"QPushButton#share_list_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_en.png);"
		"}"
	);

	folderstrulist->getfolderstructure();

}

void SecurityCloudStorageClientWidget::on_document_search_pushButton_clicked()
{
	ui->main_stackedWidget->setCurrentIndex(1);
	ui->document_management_pushButton->
		setStyleSheet(
		"QPushButton#document_management_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/shuju_mo.png);"
		"}"
		"QPushButton#document_management_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/shuju_hua.png);"
		"}"
		"QPushButton#document_management_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/shuju_en.png);"
		"}"
	);
	ui->document_search_pushButton->
		setStyleSheet(
		"QPushButton#document_search_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_en.png);"
		"}"
		"QPushButton#document_search_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_hua.png);"
		"}"
		"QPushButton#document_search_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_en.png);"
		"}"
	);
	ui->transport_list_pushButton->
		setStyleSheet(
		"QPushButton#transport_list_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_mo.png);"
		"}"
		"QPushButton#transport_list_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_hua.png);"
		"}"
		"QPushButton#transport_list_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_en.png);"
		"}"
	);

	ui->share_list_pushButton->
		setStyleSheet(
		"QPushButton#share_list_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_mo.png);"
		"}"
		"QPushButton#share_list_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_hua.png);"
		"}"
		"QPushButton#share_list_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_en.png);"
		"}"
	);
}

void SecurityCloudStorageClientWidget::on_transport_list_pushButton_clicked()
{
	ui->main_stackedWidget->setCurrentIndex(2);

	ui->document_management_pushButton->
		setStyleSheet(
		"QPushButton#document_management_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/shuju_mo.png);"
		"}"
		"QPushButton#document_management_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/shuju_hua.png);"
		"}"
		"QPushButton#document_management_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/shuju_en.png);"
		"}"
	);
	ui->document_search_pushButton->
		setStyleSheet(
		"QPushButton#document_search_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_mo.png);"
		"}"
		"QPushButton#document_search_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_hua.png);"
		"}"
		"QPushButton#document_search_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_en.png);"
		"}"
	);
	ui->transport_list_pushButton->
		setStyleSheet(
		"QPushButton#transport_list_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_en.png);"
		"}"
		"QPushButton#transport_list_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_hua.png);"
		"}"
		"QPushButton#transport_list_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_en.png);"
		"}"
	);

	ui->share_list_pushButton->
		setStyleSheet(
		"QPushButton#share_list_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_mo.png);"
		"}"
		"QPushButton#share_list_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_hua.png);"
		"}"
		"QPushButton#share_list_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_en.png);"
		"}"
	);
}

void SecurityCloudStorageClientWidget::on_share_list_pushButton_clicked()
{
	ui->main_stackedWidget->setCurrentIndex(3);
	ui->document_management_pushButton->
		setStyleSheet(
		"QPushButton#document_management_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/shuju_mo.png);"
		"}"
		"QPushButton#document_management_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/shuju_hua.png);"
		"}"
		"QPushButton#document_management_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/shuju_en.png);"
		"}"
	);
	ui->document_search_pushButton->
		setStyleSheet(
		"QPushButton#document_search_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_mo.png);"
		"}"
		"QPushButton#document_search_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_hua.png);"
		"}"
		"QPushButton#document_search_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/shujujiansuo_en.png);"
		"}"
	);
	ui->transport_list_pushButton->
		setStyleSheet(
		"QPushButton#transport_list_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_mo.png);"
		"}"
		"QPushButton#transport_list_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_hua.png);"
		"}"
		"QPushButton#transport_list_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/chuanshu_en.png);"
		"}"
	);

	ui->share_list_pushButton->
		setStyleSheet(
		"QPushButton#share_list_pushButton{"
		"border-radius:0px;"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_en.png);"
		"}"
		"QPushButton#share_list_pushButton:hover{"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_hua.png);"
		"}"
		"QPushButton#share_list_pushButton:pressed{"
		"background-image: url(:/pic/resource/img/titlebar/fenxiang_en.png);"
		"}"
	);
}

void SecurityCloudStorageClientWidget::on_fileupload_pushButton_clicked()
{
	QString parentid = "根目录";
	QFileDialog fileDialog(this);
	fileDialog.setFileMode(QFileDialog::ExistingFiles);//设置可以同时选择多个文件
	//    QString uploadFile = QFileDialog::getOpenFileName(this,QString(),QString(),"*");
	QStringList filters;
	filters
		//            << "Image files (*.png *.xpm *.jpg)"
		//    << "Text files (*.txt)"
		<< "所有文件(*)";
	fileDialog.setNameFilters(filters); //这个函数名称后面多了一个s，表示设置多个过滤器。要看清楚。
	//弹出对话框
	if (fileDialog.exec() == QDialog::Accepted) {
		//strPathList  返回值是一个list，如果是单个文件选择的话，只要取出第一个来就行了。
		QStringList strPathList = fileDialog.selectedFiles();
		//遍历
		for (int i = 0; i < strPathList.size(); ++i) {
			QString plaintextpath = strPathList.at(i);
			UploadFileEntity *uploadfileentity = new UploadFileEntity(plaintextpath, parentid);
			uploadentityhash->insert(plaintextpath, uploadfileentity);
		}
		refreshuploadlistview();
        uploadentityhash->clear();
	}
#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_

	QString logmessage;
	QTextStream logmessageout(&logmessage);
	logmessageout << "uploadentityhash size is " << uploadentityhash->size();
	SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage, 1);
#endif
}

void SecurityCloudStorageClientWidget::refreshuploadlistview()
{
#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_
	QString logmessage;
	QTextStream logmessageout(&logmessage);
	logmessageout << "in refreshuploadlistview";
	SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage, 1);
#endif
	//    qDebug()<<Q_FUNC_INFO<<"in refreshuploadlistview";
	if (nullptr != uploadentityhash && uploadentityhash->size() > 0) {
		QMap<QString, UploadFileEntity *>::const_iterator iterater = uploadentityhash->constBegin();
		while (iterater != uploadentityhash->constEnd()) {
			//            qDebug()<<Q_FUNC_INFO<<"in while";
			QString uploadfilelistkey = iterater.key();
			UploadFileEntity *uploadfileentity = iterater.value();
			QListWidgetItem *uploaditem = uploadfileentity->getuploaditem();
			uploaditem->setSizeHint(QSize(uploaditem->sizeHint().width(), 60));
			UploadListForm *uploadlistform = uploadfileentity->getuploadlistform();
			ui->uploadlist_listWidget->addItem(uploaditem);
			ui->uploadlist_listWidget->setItemWidget(uploaditem, uploadlistform);
			uploadfileentity->start();
			iterater++;
		}
	}
}

void SecurityCloudStorageClientWidget::on_filedownload_pushButton_clicked()
{
//    QString saveFileName = QFileDialog::getSaveFileName(
//                this,"save","filename",
//                QString(tr("*")),
//                Q_NULLPTR,
//                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	file_path = QFileDialog::getExistingDirectory(
		this,
		"请选择保存路径",
		"./"
	);
	if (file_path.isEmpty()) {
		return;
	}
	else {
		qDebug() << file_path << endl;
	}

	//    qDebug()<<"saveFileName ="<< saveFileName;

	//    QString decrept_key = "351a16739c1c40458311498ab7cdc86c1a3e1b077e5b4560a28fb109217b6023";
	//    QString ciphertextpath = "C:\\Users\\root\\Desktop\\1111.cipher";
	//    QString plaintextpath = "C:\\Users\\root\\Desktop\\1111.txt";

	//    string decrypt_ret =  YunLock_DecryptFile
	//            (ciphertextpath.toStdString(),
	//             plaintextpath.toStdString(),
	//             decrept_key.toStdString());
	//    QString decrypt_retqstr = QString::fromStdString(decrypt_ret);
	//    qDebug()<<"decrypt_retqstr ="<< decrypt_retqstr;

	filestrulist->batch_download();
}

void SecurityCloudStorageClientWidget::on_search_tabWidget_goto_advanced_pushButton_clicked()
{
	ui->normal_search_widget->setVisible(false);
	ui->advanced_search_widget->setVisible(true);
}

void SecurityCloudStorageClientWidget::on_search_tabWidget_goto_normal_pushButton_clicked()
{
	ui->normal_search_widget->setVisible(true);
	ui->advanced_search_widget->setVisible(false);
}
void SecurityCloudStorageClientWidget::keyReleaseEvent(QKeyEvent *event)
{
	qDebug() << Q_FUNC_INFO << "keyReleaseEvent";
	if (nullptr != event && Qt::Key_F3 == event->key()) {
		qDebug() << Q_FUNC_INFO << "F3 pressed";
		if (nullptr == locallogdialog) {
			locallogdialog = new LocalLogDialog();
		}
		if (!locallogdialog->isActiveWindow()) {
			locallogdialog->show();
		}
		if (!locallogdialog->isVisible()) {
			locallogdialog->setVisible(true);
		}
	}
	else {
		//QWidget::keyReleaseEvent(event);
	}
}

void SecurityCloudStorageClientWidget::on_refresh_pushButton_clicked()
{
#ifdef _SKLOIS_CONTENTSCROOL_DEBUG_
	//这个是日志打印。
	QString logmessage;
	QTextStream logmessageout(&logmessage);
	logmessageout << "on_refresh_pushButton_clicked";
	SecCloudLog::PrintLog(Q_FUNC_INFO, logmessage, 1);
#endif
	filestrulist->getfilemetadata("根目录", this->current_page, this->page_size);
}

void SecurityCloudStorageClientWidget::on_search_tabWidget_search_pushButton_clicked()
{
	QString keywords = this->searchpage_search_linedit->text();
	if (nullptr == keywords || keywords.length() <= 0) {
		ui->notice_label->setVisible(true);
		ui->notice_label->setText("检索关键词为空");
		return;
	}
	QString mode;
	if (0 == ui->search_tabWidget_search_pushButton->text().compare("文件名检索")) {
		mode = "filename";
	}
	else if (0 == ui->search_tabWidget_search_pushButton->text().compare("密文检索")) {
		mode = "cipherindex";
	}
	else {
		ui->notice_label->setVisible(true);
		ui->notice_label->setText("检索模式错误");
		return;
	}
	ui->notice_label->setVisible(false);
	ui->notice_label->setText("");
	SearchResultWidget *searchresultwidget = new SearchResultWidget();
	searchresultwidget->data_search(
		ui->search_tabWidget,
		keywords,
		"",
		mode,
		QDateTime(),
		QDateTime()
	);
}

void SecurityCloudStorageClientWidget::on_search_tabWidget_advanced_search_pushButton_clicked()
{
	QString keywords = ui->key_words_lineEdit->text();
	if (nullptr == keywords || keywords.length() <= 0) {
		ui->notice_label->setVisible(true);
		ui->notice_label->setText("检索关键词为空");
		return;
	}
	QString type;
	if (Qt::Checked == ui->other_checkBox->checkState()) {
		type = "*";
	}
	else {
		if (Qt::Checked == ui->picture_checkBox->checkState()) {
			type += "p";
		}
		if (Qt::Checked == ui->ducument_checkBox->checkState()) {
			type += "d";
		}
		if (Qt::Checked == ui->video_checkBox->checkState()) {
			type += "v";
		}
		if (Qt::Checked == ui->music_checkBox->checkState()) {
			type += "m";
		}
		if (Qt::Unchecked == ui->picture_checkBox->checkState() &&
			Qt::Unchecked == ui->ducument_checkBox->checkState() &&
			Qt::Unchecked == ui->video_checkBox->checkState() &&
			Qt::Unchecked == ui->music_checkBox->checkState()) {
			ui->notice_label->setVisible(true);
			ui->notice_label->setText("请勾选检索类型");
			return;
		}
	}

	QString mode;
	if (ui->filenamesearch_radioButton->isChecked()) {
		mode = "filename";
	}
	else if (ui->ciphersearch_radioButton->isChecked()) {
		mode = "cipherindex";
	}
	else {
		ui->notice_label->setVisible(true);
		ui->notice_label->setText("请选择检索方式");
		return;
	}

	QDateTime fromtime = ui->advanced_from_dateTimeEdit->dateTime();
	QDateTime totime = ui->advanced_to_dateTimeEdit->dateTime();
	ui->notice_label->setVisible(false);
	ui->notice_label->setText("");
	SearchResultWidget *searchresultwidget = new SearchResultWidget();
	searchresultwidget->data_search(
		ui->search_tabWidget,
		keywords,
		type,
		mode,
		fromtime,
		totime
	);
}

void SecurityCloudStorageClientWidget::slot_search_result_tabwidget_closed(int index)
{
	ui->search_tabWidget->removeTab(index);
}
