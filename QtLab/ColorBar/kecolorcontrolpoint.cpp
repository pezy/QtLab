#include "kecolorcontrolpoint.h"
#include <QPainter>

CKEColorControlPoint::CKEColorControlPoint(EPositionType posType)
    : QGraphicsItem()
    , m_posType(posType)
    , m_color(Qt::white)
{

}

CKEColorControlPoint::~CKEColorControlPoint()
{

}

QRectF CKEColorControlPoint::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-5 - penWidth / 2, -5 - penWidth / 2, 5 + penWidth / 2, 5 + penWidth / 2);
}

void CKEColorControlPoint::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(m_color);

    QPolygonF triangle;

    if (m_posType == eTop)
    {
        triangle.push_back(QPointF(0.f, 10.f));
        triangle.push_back(QPointF(-5.f, 0.f));
        triangle.push_back(QPointF(5.f, 0.f));
    }
    else if (m_posType == eBottom)
    {
        triangle.push_back(QPointF(0.f, 0.f));
        triangle.push_back(QPointF(-5.f, 10.f));
        triangle.push_back(QPointF(5.f, 10.f));
    }

    painter->drawPolygon(triangle);
}