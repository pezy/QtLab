#include "kecolorcontrolpoint.h"
#include <QPainter>
#include <QGraphicsScene>
#include "kecolormapbar.h"
#include "kecolormap.h"

CKEColorControlPoint::CKEColorControlPoint(EPositionType posType, int index, CKEColormap* pColormap)
    : QGraphicsItem()
    , m_posType(posType)
    , m_index(index)
    , m_pColormap(pColormap)
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
    painter->setBrush(m_pColormap->GetColorAt(m_index));

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