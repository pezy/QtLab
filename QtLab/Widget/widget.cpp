#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{
    
}

void Widget::paintEvent(QPaintEvent *event)
{
	// childrenRect()
	QRect childrenRect = this->childrenRect();
	qDebug() << "childrenRect: " << childrenRect.width() << ", " << childrenRect.height();

	// contentsRect()
	QRect contentRect = this->contentsRect();
	qDebug() << "contentRect: " << contentRect.width() << ", " << contentRect.height();

	// frameGeometry()
	QRect frameGeom = this->frameGeometry();
	qDebug() << "frameGeom" << frameGeom.width() << ", " << frameGeom.height();

	// geometry()
	QRect geom = this->geometry();
	qDebug() << "Geometry: " << geom.width() << ", " << geom.height();

	// normalGeometry()
	QRect normalGeom = this->normalGeometry();
	qDebug() << "normalGeom: " << normalGeom.width() << ", " << normalGeom.height();

	// rect()
	QRect rect = this->rect();
	qDebug() << "rect: " << rect.width() << ", " << rect.height();

	// Directly : width() and heigth()
	qDebug() << "Directly: " << this->width() << ", " << this->height();
}