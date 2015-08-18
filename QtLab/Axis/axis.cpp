#include "axis.h"
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QTextItem>
#include <QScrollBar>

Axis::Axis(QWidget *parent)
	: QWidget(parent)
{
	ui_.setupUi(this);

	scene_ = new QGraphicsScene;
	scene_->addText("Kerogen");

	for (auto i = 0; i != 32; ++i)
	{
		auto item_y = new QGraphicsLineItem();
		item_y->setPen(QPen(Qt::red));
		item_y->setLine(0, i * 20, 10, i * 20);
		scene_->addItem(item_y);

		auto item_x = new QGraphicsLineItem();
		item_x->setPen(QPen(Qt::red));
		item_x->setLine(i * 20, 0, i * 20 + 10, 0);
		scene_->addItem(item_x);
	}

	ui_.m_view->setScene(scene_);
}

Axis::~Axis()
{

}

void Axis::SetMarginY(int y)
{
	margin_y_ = y;
	update();
}

int Axis::GetMarginY()
{
	return margin_y_;
}

void Axis::SetMarginX(int x)
{
	margin_x_ = x;
	update();
}

int Axis::GetMarginX()
{
	return margin_x_;
}

void Axis::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	QFontMetrics font_metrics(font());
	int offset_y = font_metrics.height() / 2;
	
	for (auto i = 0; i != 32; ++i)
	{
		int y = margin_y_ + i * 20 + ui_.m_view->y() + offset_y;
		if (ui_.m_view->y() + ui_.m_view->height() > y && y > ui_.m_view->y()) 
			painter.drawText(10, y, QString::number(i + 1));

		int mask_width = font_metrics.width(QString::number(i + 1));
		int offset_x = mask_width < 10 ? mask_width / 2 : 0;
		int x = margin_x_ + i * 20 + ui_.m_view->x() + offset_x;
		if (ui_.m_view->x() + ui_.m_view->width() > x && x > ui_.m_view->x())
			painter.drawText(x, 20, QString::number(i + 1));
	}
}