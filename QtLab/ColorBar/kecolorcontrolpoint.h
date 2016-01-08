#ifndef KECOLORCONTROLPOINT_H
#define KECOLORCONTROLPOINT_H

#include <QGraphicsItem>

class CKEColormap;

class CKEColorControlPoint : public QGraphicsItem
{
public:
    enum EPositionType{eTop, eBottom};

    CKEColorControlPoint(EPositionType posType, int index, CKEColormap* pColormap);
    ~CKEColorControlPoint();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    int GetIndex() const { return m_index; }
    void SetIndex(int index) { m_index = index; }

    EPositionType GetType() const { return m_posType; }
    void SetType(EPositionType type) { m_posType = type; }

private:
    EPositionType m_posType;
    int m_index;
    CKEColormap* m_pColormap;
};

#endif // KECOLORCONTROLPOINT_H
