#include "kecolormapbarscene.h"

#include <QPainter>
#include "kecolormap.h"
#include "kecolorcontrolpoint.h"

CKEColormapBarScene::CKEColormapBarScene(QObject *parent)
    : QGraphicsScene(parent)
{
    CKEColormap::InitializeColormaps();
    m_colormap = CKEColormap::GetColormap("rainbow");

    //m_pBottomLeft = new CKEColorControlPoint(CKEColorControlPoint::EPositionType::eBottom);
    //m_pBottomRight = new CKEColorControlPoint(CKEColorControlPoint::EPositionType::eBottom);

    //addItem(m_pBottomLeft);
    //addItem(m_pBottomRight);
}

CKEColormapBarScene::~CKEColormapBarScene()
{

}

void CKEColormapBarScene::drawBackground(QPainter * pPainter, const QRectF & rect)
{
    pPainter->save();
    QRectF colorRect = rect.adjusted(5.f, 15.f, -5.f, -15.f);
    for (int x = 0; x != colorRect.width(); ++x)
    {
        pPainter->setPen(m_colormap->GetColorAt(qRound(1.f / colorRect.width() * x * 255)));
        pPainter->drawLine(x + colorRect.left(), colorRect.top(), x + colorRect.left(), colorRect.bottom());
    }

    //m_pBottomLeft->setPos(colorRect.left(), colorRect.bottom());
    //m_pBottomLeft->SetColor(m_colormap->GetColorAt(0));
    //m_pBottomRight->setPos(colorRect.right(), colorRect.bottom());
    //m_pBottomRight->SetColor(m_colormap->GetColorAt(255));

    pPainter->restore();
}