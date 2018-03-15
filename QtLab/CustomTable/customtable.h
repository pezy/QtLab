#ifndef CUSTOMTABLE_H
#define CUSTOMTABLE_H

#include <QtWidgets/QMainWindow>
#include "ui_customtable.h"

class CustomTable : public QMainWindow
{
	Q_OBJECT

public:
	CustomTable(QWidget *parent = 0);
	~CustomTable();

private:
	Ui::CustomTableClass ui;
};

#endif // CUSTOMTABLE_H
