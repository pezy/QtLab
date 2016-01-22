#include "kecolortagitem.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

CKEColorTagItem::CKEColorTagItem(int index, TagType type, QColor color, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
    , m_index(index)
    , m_type(type)
    , m_color(color)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setBrush(QBrush(color));
    setPen(QPen(Qt::black));
    
    if (type == SingleColor)
    {
        QPolygonF triangle;
        triangle << QPointF(-5, 0) << QPointF(5, 0) << QPointF(0, 10);
        setPolygon(triangle);
    }
    else if (type == InterpolatingColor)
    {
        QPolygonF invertedTriangle;
        invertedTriangle << QPointF(-5, 0) << QPointF(5, 0) << QPointF(0, -10);
        setPolygon(invertedTriangle);
    }
}

CKEColorTagItem::~CKEColorTagItem()
{

}

QVariant CKEColorTagItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange && scene()) 
    {
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        if (!rect.contains(newPos))
        {
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}