#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "item.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Item *pRectItem = new Item(QRectF(QPointF(),QSize(100,150)));

    QGraphicsScene *pScene = new QGraphicsScene;
    pScene->addItem(pRectItem);

    View *pView = new View(pScene);
    pView->show();

    return a.exec();
}
