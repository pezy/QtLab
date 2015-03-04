#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
class QGraphicsScene;
class QMouseEvent;

class View : public QGraphicsView
{
public:
    View(QGraphicsScene * scene);
    ~View();

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
};

#endif // VIEW_H
