#ifndef SECURITYCLOUDSTORAGECLIENTWIDGET_H
#define SECURITYCLOUDSTORAGECLIENTWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>
#include <QLabel>
#include <QMenu>
#include <QSequentialAnimationGroup>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QUuid>
#include <QProcess>
#include <QListWidgetItem>
#include <QThread>
#include "./operationwidget.h"
#include "./local_lineedit/local_lineedit.h"
#include "global.h"
#include "./lib/include/cipherinterface.h"
#include "transformentity/uploadfileentity.h"
#include "./transformlistform/uploadlistform.h"
#include "./transformlistform/downloadlistform.h"
#include "./entities/localtreewidgetitem.h"
#include "./FolderStructureList/folderstructurelist.h"
#include "./FileStructureList/filestructurelist.h"
#include "./LocalLog/seccloudlog.h"
#include "./LocalLog/locallogdialog.h"
#include "./OnlineUpdate/onlineupdatedialog.h"
#include "./FileShare/getsharelist.h"
#include "./FileShare/getsharedlist.h"
#include "./SearchResult/searchresultwidget.h"
#include <QtTest>
#include <csignal>

namespace Ui
{
class SecurityCloudStorageClientWidget;
}

class SecurityCloudStorageClientWidget: public QWidget
{
Q_OBJECT

public:
	explicit SecurityCloudStorageClientWidget(QWidget *parent = nullptr);
	~SecurityCloudStorageClientWidget();

private slots:
	void on_document_management_pushButton_clicked();
	void on_transport_list_pushButton_clicked();
	void on_share_list_pushButton_clicked();
	void on_document_search_pushButton_clicked();
	void slot_jumptopage_linedit_focussed(bool);
	void slot_searchpage_search_linedit_focussed(bool);
	void on_fileupload_pushButton_clicked();
	void on_filedownload_pushButton_clicked();
	void slot_namesearch_clicked();
	void slot_ciphersearch_clicked();

	void slot_search_tabWidget_namesearch_clicked();
	void slot_search_tabWidget_ciphersearch_clicked();

	void on_search_tabWidget_goto_advanced_pushButton_clicked();
	void on_search_tabWidget_goto_normal_pushButton_clicked();
	void refreshuploadlistview();

	void TreeWidgetItemPressed_Slot(QTreeWidgetItem *pressedItem, int column);

	void on_refresh_pushButton_clicked();

	void slot_datashare_tabwidget_clicked(int);

	void on_search_tabWidget_search_pushButton_clicked();

	void on_search_tabWidget_advanced_search_pushButton_clicked();

	void slot_search_result_tabwidget_closed(int);
protected:
	void keyReleaseEvent(QKeyEvent *event);
private:
	Ui::SecurityCloudStorageClientWidget *ui;

	Local_LineEdit *local_linedit{local_linedit = nullptr};

	Local_LineEdit *searchpage_search_linedit{searchpage_search_linedit = nullptr};

	int current_page = 0;
	int page_size = 0;

	QString doc_plaintext = "";

	QMap<QString, UploadFileEntity *> *uploadentityhash{uploadentityhash = nullptr};

	FolderStructureList *folderstrulist{folderstrulist = nullptr};
	FileStructureList *filestrulist{filestrulist = nullptr};
};

#endif // SECURITYCLOUDSTORAGECLIENTWIDGET_H
