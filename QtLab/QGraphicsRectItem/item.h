#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsRectItem>
class QGraphicsSceneMouseEvent;

class Item : public QGraphicsRectItem
{
public:
    Item(const QRectF & rect, QGraphicsItem * parent = 0);
    ~Item();

    void debugPrint();
};

#endif // ITEM_H
