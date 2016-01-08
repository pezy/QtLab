#include "kecolormapbar.h"

#include <QGraphicsView>
#include <QResizeEvent>

#include "kecolormap.h"
#include "kecolormapbarview.h"
#include "kecolorcontrolpoint.h"

CKEColormapBar::CKEColormapBar(QWidget *parent)
    : QWidget(parent)
{
    _InitData();
    _Init();

    setMinimumSize(400, 50);
}

CKEColormapBar::~CKEColormapBar()
{

}

void CKEColormapBar::resizeEvent(QResizeEvent *event)
{
    m_pView->setFixedSize(event->size());
}

void CKEColormapBar::_Init()
{
    m_pView = new CKEColormapBarView(&m_pScene, this);
    CKEColorControlPoint pointMin(CKEColorControlPoint::EPositionType::eBottom, 0, m_colormap);
    CKEColorControlPoint pointMax(CKEColorControlPoint::EPositionType::eBottom, 255, m_colormap);

    m_pScene.addItem(&pointMin);
    m_pScene.addItem(&pointMax);

    QPointF posMin = GetPosByItem(pointMin);
    QPointF posMax = GetPosByItem(pointMax);

    pointMin.setPos(0, 100);
    pointMax.setPos(posMax);
}

void CKEColormapBar::_InitData()
{
    CKEColormap::InitializeColormaps();
    m_colormap = CKEColormap::GetColormap("rainbow");
}

QPointF CKEColormapBar::GetPosByItem(const CKEColorControlPoint& item) const
{
    if (item.GetType() == CKEColorControlPoint::EPositionType::eBottom)
    {
        return QPointF(5.f + item.GetIndex(), 40);
    }
    else
    {
        return QPointF(5.f + item.GetIndex(), 10);
    }
}