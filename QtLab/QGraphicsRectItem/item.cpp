#include "item.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

Item::Item(const QRectF & rect, QGraphicsItem * parent)
    :QGraphicsRectItem(rect, parent)
{
}

Item::~Item()
{
}

void Item::debugPrint()
{
    qDebug() << "do something in Item.";
}
