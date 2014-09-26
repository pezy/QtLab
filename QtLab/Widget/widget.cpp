#include "widget.h"
#include <QDebug>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{
    
}

void Widget::paintEvent(QPaintEvent *event)
{
	/*!
	 * Draw the border
	 */
	QPainter painter(this);
	QRect rect = this->geometry();
	rect.moveTopLeft(QPoint(0, 0));
	rect.adjust(10, 10, -10, -10);
	qDebug() << rect.x() << "|" << rect.y() << "|" << rect.bottom() << "," << rect.top() << "," << rect.left() << "," << rect.right();
	painter.drawRect(rect);

	/*!
	 * Draw the slip
	 */
	painter.setPen(QPen(Qt::red, 2));
	painter.drawLine(QPoint(rect.left()+5, rect.top()), QPoint(rect.left()+5, rect.bottom()));
	painter.drawLine(QPoint(rect.right()-5, rect.top()), QPoint(rect.right()-5, rect.bottom()));
}