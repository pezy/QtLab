#include "view.h"
#include "axis.h"

View::View(QWidget *parent)
	: QGraphicsView(parent)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setRenderHint(QPainter::Antialiasing);
}

View::~View()
{

}

void View::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy(dx, dy);

	Axis *axis = dynamic_cast<Axis*>(parent());
	if (axis)
	{
		axis->SetMarginY(axis->GetMarginY() + dy);
		axis->SetMarginX(axis->GetMarginX() + dx);
	}
}