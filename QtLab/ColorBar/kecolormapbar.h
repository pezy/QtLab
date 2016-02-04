#ifndef KECOLORMAPBAR_H
#define KECOLORMAPBAR_H

#include <QWidget>

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

    CKEColormap* GetColormap() { return m_pColormap; }

public slots:
    void slotReset();
    void slotFlip();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    QSize sizeHint() const override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    void _InitData();
    void _DrawControlPoint(int index);
    void _DrawControlRange(int beg, int end);
    void _DrawInterpolationMiddlePoint(int index);

    QPointF _ColorIndexToControlPos(int index);
    QPointF _ColorIndexToInterpolationPos(int index);
    int _PosToColorIndex(const QPointF& pos);

    void _SetSingleColor(const QPointF& position);
    void _SelectControlPoint(const QPointF& position);
    void _SetControlPoint(const QPointF& position);
    void _SetMiddlePoint(const QPointF& position);

    void _CalcLinearInterpolation(int beg, int end);
    QColor _CalcMiddleColor(int beg, int end);

    void _ResetControlPointsList();

    void _CalcCompressionToRight(int oldIndex, int newIndex);
    void _CalcCompressionToLeft(int oldIndex, int newIndex);

    bool _CheckBoundingRange(const QPointF& position);

private:
    CKEColormap* m_pColormap = nullptr;

    const int m_nColor = 256;
    const qreal m_marginHorizon = 5.0;
    const qreal m_marginVerticle = 15.0;

    qreal m_singleColorBarWidth = 0.0;
    QRectF m_colorBarRect;
    QRectF m_upperBarRect;
    QRectF m_lowerBarRect;

    QList<int> m_listControlPoints;
    QPair<int, int> m_interpolationRange;

    int m_interpolationMiddlePoint = -1;
    int m_selectedPointIndex = -1;
    int m_compressionIndex = -1;

    bool m_bStartCompression = false;
};

#endif // KECOLORMAPBAR_H
