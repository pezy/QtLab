
#include <QtGui/QApplication>
#include "plotter.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Plotter plotter;
	plotter.show();
	return a.exec();
}
