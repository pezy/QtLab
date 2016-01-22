#ifndef KECOLORTAGITEM_H
#define KECOLORTAGITEM_H

#include <QGraphicsPolygonItem>

/*!
*  @class  CKEColorTagItem in "kecolortagitem.h"
*  @brief  color tag for editing color rainbow.
*  @author pezy
*  @date   2016-01-22
*/

class CKEColorTagItem : public QGraphicsPolygonItem
{
public:
    enum TagType {SingleColor = UserType + 1, InterpolatingColor};

    CKEColorTagItem(int index, TagType type, QColor color, QGraphicsItem *parent = 0);
    ~CKEColorTagItem();

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
    int m_index;
    QColor m_color;
    TagType m_type;
};

#endif // KECOLORTAGITEM_H
