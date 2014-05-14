
#include <QtGui/QApplication>
#include "iconeditor.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	IconEditor iconEditor;
	iconEditor.show();
	return a.exec();
}
