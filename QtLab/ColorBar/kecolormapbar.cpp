#include "kecolormapbar.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QResizeEvent>

#include "kecolormap.h"
#include "kecolortagitem.h"

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
    m_pScene = new QGraphicsScene;
    m_pView = new QGraphicsView(m_pScene, this);
    m_pView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    CKEColorTagItem *pItem = new CKEColorTagItem(0, CKEColorTagItem::SingleColor, Qt::red);
    m_pScene->addItem(pItem);
}

void CKEColormapBar::_InitData()
{
    CKEColormap::InitializeColormaps();
    m_colormap = CKEColormap::GetColormap("rainbow");
}