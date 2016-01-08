#include "kecolormapbarview.h"
#include "kecolormapbar.h"
#include "kecolormap.h"

CKEColormapBarView::CKEColormapBarView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    m_pBar = dynamic_cast<CKEColormapBar*>(parent);

    this->setRenderHint(QPainter::Antialiasing);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

CKEColormapBarView::~CKEColormapBarView()
{

}

void CKEColormapBarView::drawBackground(QPainter *painter, const QRectF &rect)
{
    CKEColormap *colormap = m_pBar->GetColormap();

    painter->save();
    QRectF colorRect = rect.adjusted(5.f, 15.f, -5.f, -15.f);
    for (int x = 0; x != colorRect.width(); ++x)
    {
        painter->setPen(colormap->GetColorAt(qRound(1.f / colorRect.width() * x * 255)));
        painter->drawLine(x + colorRect.left(), colorRect.top(), x + colorRect.left(), colorRect.bottom());
    }

    painter->restore();
}