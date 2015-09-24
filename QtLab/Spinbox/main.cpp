#include <QApplication>
#include "qmainwidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QMainWidget w;
	w.show();
	return a.exec();
}
