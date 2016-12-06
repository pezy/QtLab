#ifndef RANDOMCOLOR_H
#define RANDOMCOLOR_H

#include <QtWidgets/QMainWindow>
#include "ui_randomcolor.h"

class RandomColor : public QMainWindow
{
	Q_OBJECT

public:
	RandomColor(QWidget *parent = 0);
	~RandomColor();

private:
	void _Init();
	void _SetLabelColor(QLabel* pLabel, const QColor& color);

private slots:
	void slotRandom();
	void slotKellyColors();

private:
	Ui::RandomColorClass m_ui;
};

#endif // RANDOMCOLOR_H
