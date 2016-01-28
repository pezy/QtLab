#ifndef KECOLORMAPBAR_H
#define KECOLORMAPBAR_H

#include <QWidget>
#include <QMap>

/*!
 *  @class  CKEColormapBar in kecolormapbar.h
 *  @brief  widget for editing color rainbow.
 *  @author pezy
 *  @date   2016-01-04
 */

class CKEColormap;

class CKEColormapBar : public QWidget
{
    Q_OBJECT

public:
    explicit CKEColormapBar(QWidget *parent = 0);
    ~CKEColormapBar();

    CKEColormap* GetColormap() const { return m_colormap; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    QSize sizeHint() const override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void _Init();
    void _InitData();
    void _DrawControlPoint(int index);
    void _DrawInterpolationIndex(int index);
    QPointF _ColorIndexToControlPos(int index);
    QPointF _ColorIndexToInterpolationPos(int index);
    int _PosToColorIndex(const QPointF& pos);
    int _ControlPointIndexAtPos(QPointF pos);

    void _ChangeSingleColor(const QPointF& position);
    void _ColorInterpolation();

private:
    CKEColormap* m_colormap = nullptr;

    const int m_nColor = 256;
    const qreal m_marginHorizon = 5.0;
    const qreal m_marginVerticle = 15.0;

    qreal m_singleColorBarWidth = 0.0;
    QRectF m_colorBarRect;
    QRectF m_upperBarRect;
    QRectF m_lowerBarRect;

    QMap<int, QRectF> m_mapCPRect; // Control points bounding rect.
    int m_highlightIndex = -1;

    int m_interpolationIndex = -1;
    QRectF m_rectInterpolation;
};

#endif // KECOLORMAPBAR_H
