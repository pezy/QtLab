#include "workbench.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WorkBench w;
	w.show();
	return a.exec();
}
