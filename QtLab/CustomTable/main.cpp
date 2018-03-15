#include "customtable.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CustomTable w;
	w.show();
	return a.exec();
}
