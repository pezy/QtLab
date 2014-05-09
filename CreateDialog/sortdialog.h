#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>

#include "ui_sortdialog.h"

class SortDialog : public QDialog, public Ui::SortDialog
{
	Q_OBJECT

public:
	SortDialog(QWidget *parent = 0);
	~SortDialog();
	void setColumnRange(QChar first, QChar last);

private:
	
};

#endif // SORTDIALOG_H
