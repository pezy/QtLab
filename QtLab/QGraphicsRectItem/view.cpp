#include "view.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include "item.h"

View::View(QGraphicsScene * scene) : QGraphicsView(scene)
{

}

View::~View()
{

}

void View::mousePressEvent( QMouseEvent * event)
{
    qDebug() << "press";

    QGraphicsItem *pItem = itemAt(event->pos());
    if (pItem) {
        Item *rectItem = dynamic_cast<Item*>(pItem);
        rectItem->debugPrint();
    }
}

void View::mouseMoveEvent( QMouseEvent * )
{
    qDebug() << "move";
}

