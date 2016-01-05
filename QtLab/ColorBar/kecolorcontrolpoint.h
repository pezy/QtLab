#ifndef KECOLORCONTROLPOINT_H
#define KECOLORCONTROLPOINT_H

#include <QGraphicsItem>

class CKEColorControlPoint : public QGraphicsItem
{
public:
    enum EPositionType{eTop, eBottom};

    CKEColorControlPoint(EPositionType posType);
    ~CKEColorControlPoint();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void SetColor(const QColor& color) { m_color = color; }

private:
    EPositionType m_posType;
    QColor m_color;
};

#endif // KECOLORCONTROLPOINT_H
