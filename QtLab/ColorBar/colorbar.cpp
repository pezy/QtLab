#include "colorbar.h"

#include <QGraphicsView>
#include <QGridLayout>
#include "kecolormapbarscene.h"

ColorBar::ColorBar(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    CKEColormapBarScene* pScene = new CKEColormapBarScene(this);
    QGraphicsView *pView = new QGraphicsView(pScene, this);
    pView->setRenderHint(QPainter::Antialiasing);
    pView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    this->setCentralWidget(pView);
    //QGridLayout* pLayout = new QGridLayout(this);
    //pLayout->addWidget(pView);
    //setLayout(pLayout);
}

ColorBar::~ColorBar()
{

}
