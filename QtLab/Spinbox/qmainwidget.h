#ifndef QMAINWIDGET_H
#define QMAINWIDGET_H

#include <QWidget>
#include "GeneratedFiles/ui_qmainwidget.h"

class QMainWidget : public QWidget
{
	Q_OBJECT

public:
	QMainWidget(QWidget *parent = 0);
	~QMainWidget();

	private slots:
	void slotSetValue(int value);
	void slotReset();

private:
	Ui::QMainWidget ui;
};

#endif // QMAINWIDGET_H
