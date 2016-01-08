#ifndef KECOLORMAPBARVIEW_H
#define KECOLORMAPBARVIEW_H

#include <QGraphicsView>

class CKEColormapBar;

class CKEColormapBarView : public QGraphicsView
{
    Q_OBJECT

public:
    CKEColormapBarView(QGraphicsScene *scene, QWidget *parent);
    ~CKEColormapBarView();

    CKEColormapBarView(const CKEColormapBarView&) = delete;
    CKEColormapBarView& operator=(const CKEColormapBarView&) = delete;

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

private:
    CKEColormapBar* m_pBar;
};

#endif // KECOLORMAPBARVIEW_H
