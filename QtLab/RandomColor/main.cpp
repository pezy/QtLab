#include "randomcolor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RandomColor w;
	w.show();
	return a.exec();
}
