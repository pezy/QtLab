#ifndef AXIS_H
#define AXIS_H

#include <QtWidgets/QWidget>
#include "ui_axis.h"

class QGraphicsScene;

class Axis : public QWidget
{
	Q_OBJECT

public:
	Axis(QWidget *parent = 0);
	~Axis();

	void SetMarginY(int y);
	int GetMarginY();

	void SetMarginX(int x);
	int GetMarginX();

protected:
	virtual void paintEvent(QPaintEvent * event) override;

private:
	Ui::AxisClass ui_;
	QGraphicsScene *scene_;

	int margin_x_ = 0;
	int margin_y_ = 0;
};

#endif // AXIS_H
