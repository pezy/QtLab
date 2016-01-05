#ifndef KECOLORMAPBARSCENE_H
#define KECOLORMAPBARSCENE_H

#include <QGraphicsScene>

class CKEColormap;
class CKEColorControlPoint;

class CKEColormapBarScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CKEColormapBarScene(QObject *parent = nullptr);
    ~CKEColormapBarScene();

protected:
    virtual void drawBackground(QPainter * painter, const QRectF & rect);

private:
    CKEColormap* m_colormap;
    CKEColorControlPoint *m_pBottomLeft;
    CKEColorControlPoint *m_pBottomRight;
};

#endif // KECOLORMAPBARSCENE_H
