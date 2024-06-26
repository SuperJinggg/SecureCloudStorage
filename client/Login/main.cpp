#include "loginwidget.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


//    SecurityCloudStorageClientWidget w;
//    w.show();
	LoginWidget loginwidget;
	loginwidget.show();


	QFile file(QString(":/pic/resource/style.qss"));
	file.open(QFile::ReadOnly);
	QString qss = QLatin1String(file.readAll());

	qApp->setStyleSheet(qss);


	return a.exec();
}
