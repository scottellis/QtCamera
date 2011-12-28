#include "qtcamera.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtCamera w;
	w.show();
	return a.exec();
}
