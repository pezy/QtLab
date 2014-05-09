#include <QtGui/QApplication>
#include "finddialog.h"
#include "gotocelldialog.h"
#include "sortdialog.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//FindDialog *dialog = new FindDialog;
	//dialog->show();
	//GoToCellDialog *dialog = new GoToCellDialog;
	SortDialog *dialog = new SortDialog;
	dialog->setColumnRange('C', 'F');
	dialog->show();

	return a.exec();
}
