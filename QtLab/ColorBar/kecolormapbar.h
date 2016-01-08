#ifndef KECOLORMAPBAR_H
#define KECOLORMAPBAR_H

#include <QWidget>
#include <QGraphicsScene>

/*!
 *  @class  CKEColormapBar in kecolormapbar.h
 *  @brief  widget for editing color rainbow.
 *  @author pezy
 *  @date   2016-01-04
 */

class CKEColormap;
class CKEColormapBarView;
class CKEColorControlPoint;

class CKEColormapBar : public QWidget
{
    Q_OBJECT

public:
    explicit CKEColormapBar(QWidget *parent = 0);
    ~CKEColormapBar();

    CKEColormap* GetColormap() const { return m_colormap; }

protected:
    virtual void resizeEvent(QResizeEvent *event);
    
private:
    void _Init();
    void _InitData();

    QPointF GetPosByItem(const CKEColorControlPoint& item) const;

private:
    CKEColormap* m_colormap = nullptr;
    CKEColormapBarView *m_pView = nullptr;

    QGraphicsScene m_pScene;
};

#endif // KECOLORMAPBAR_H
