#include "widget.h"
#include <QDebug>
#include <QPainter>
#include <QFontMetrics>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{
    
}

void Widget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	
	/*!
	 * Draw the border
	 */
	painter.drawRect(0, 0, width()-1, height()-1);	/**< if you don't subtract 1, you can not see the border on bottom and right. */
	painter.fillRect(0, 0, width()-1, height()-1, Qt::white);

	/*!
	 * Build the coordinate
	 */

	QFontMetrics metrics = painter.fontMetrics();
	
	int leftBearing = metrics.width("100") + 5; /**< 5 is the scale width. */
	int coordWidth = width() - 2*leftBearing;
	int coordHeight = height() - 4*metrics.height(); /**< just try */

	QStringList monthList;
	monthList << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";

	QStringList valueList;
	valueList << "0.0" << "3.2" << "6.5" << "9.8" << "13.0";

	painter.translate(leftBearing, coordHeight + 1.75*metrics.height()); /**< move center to left bottom */
	float deltaX = static_cast<float>(coordWidth)/monthList.size();
	float deltaY = static_cast<float>(coordHeight)/(valueList.size()-1);

	/*!
	 * Draw the coordinate
	 */
	painter.drawLine(0, 0, coordWidth, 0);
	for (int i = 0; i != monthList.size(); ++i)
	{
		int strLen = metrics.width(monthList.at(i));
		// scale
		painter.drawLine(deltaX*(i+1), 0, deltaX*(i+1), 4);
		// text
		painter.drawText(deltaX*i + (deltaX-strLen)/2 ,metrics.height(), monthList.at(i));
	}

	painter.drawLine(0, 0, 0, -coordHeight);
	for (int i = 0; i != valueList.size(); ++i)
	{
		int deviation = metrics.height()/2 - metrics.descent();
		painter.drawLine(-4, -deltaY*i, 0, -deltaY*i);
		painter.drawText(-metrics.width(valueList.at(i))-4, -deltaY*i+deviation, valueList.at(i));
	}
}