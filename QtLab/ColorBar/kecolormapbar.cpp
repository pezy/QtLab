#include "kecolormapbar.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QColorDialog>

#include "kecolormap.h"

CKEColormapBar::CKEColormapBar(QWidget *parent)
    : QWidget(parent)
{
    _InitData();
    _ResetControlPointsList();
}

CKEColormapBar::~CKEColormapBar()
{
    delete m_pColormap;
}

void CKEColormapBar::slotReset()
{
    CKEColormap *pColormap = CKEColormap::GetColormap("rainbow");
    for (int i = 0; i != 256; ++i)
    {
        m_pColormap->SetColorAt(i, pColormap->GetColorAt(i));
    }

    _ResetControlPointsList();

    update();
}

void CKEColormapBar::slotFlip()
{
    int first = 0, last = 256;
    while (first != last && first != --last)
    {
        QColor temp = m_pColormap->GetColorAt(first);
        m_pColormap->SetColorAt(first, m_pColormap->GetColorAt(last));
        m_pColormap->SetColorAt(last, temp);
        ++first;
    }

    update();
}

void CKEColormapBar::paintEvent(QPaintEvent*)
{
    m_singleColorBarWidth = m_colorBarRect.width() / m_nColor;

    QPainter painter(this);

    for (int i = 0; i != m_nColor; ++i)
    {
        painter.setPen(m_pColormap->GetColorAt(i));
        painter.setBrush(m_pColormap->GetColorAt(i));
        painter.drawRect(i * m_singleColorBarWidth + m_marginHorizon, m_marginVerticle, m_singleColorBarWidth, m_colorBarRect.height());
    }

    // min and max
    _DrawControlPoint(0);
    _DrawControlPoint(255);

    for (auto index : m_listControlPoints)
    {
        _DrawControlPoint(index);
    }

    _DrawInterpolationMiddlePoint(m_interpolationMiddlePoint);
    _DrawControlRange(m_interpolationRange.first, m_interpolationRange.second);
}

void CKEColormapBar::resizeEvent(QResizeEvent* event)
{
    qreal newWidth = event->size().width() - 2 * m_marginHorizon;
    qreal newHeight = event->size().height() - 2 * m_marginVerticle;
    m_colorBarRect.setRect(m_marginHorizon, m_marginVerticle, newWidth, newHeight);
    m_upperBarRect.setRect(0, 0, event->size().width(), m_marginVerticle);
    m_lowerBarRect.setRect(0, event->size().height() - m_marginVerticle, event->size().width(), m_marginVerticle);
}

QSize CKEColormapBar::sizeHint() const
{
    return QSize(640, 50);
}

void CKEColormapBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton)
    {
        m_compressionIndex = _PosToColorIndex(event->pos());
        _SelectControlPoint(event->pos());
        m_bStartCompression = true;
    }
    else if (event->button() == Qt::LeftButton)
    {
        _SetSingleColor(event->pos());
        _SelectControlPoint(event->pos());
        _SetMiddlePoint(event->pos());

        update();
    }
}

void CKEColormapBar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bStartCompression)
    {
        if (!_CheckBoundingRange(event->pos()))
            return;

        _SetControlPoint(event->pos());
    }
    else
    {
        _SetMiddlePoint(event->pos());
        _SetControlPoint(event->pos());
    }

    update();
}

void CKEColormapBar::mouseReleaseEvent(QMouseEvent* event)
{
    m_selectedPointIndex = -1; 
    
    if (event->button() == Qt::MiddleButton)
    {
        int newIndex = _PosToColorIndex(event->pos());
        _CalcCompressionToRight(m_compressionIndex, newIndex);
        _CalcCompressionToLeft(m_compressionIndex, newIndex);
        m_bStartCompression = false;
    }

    update();
}

void CKEColormapBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (!m_lowerBarRect.contains(event->pos()))
        return;

    int clickedIndexBeg = _PosToColorIndex(event->pos() + QPointF(-5, 0));
    int clickedIndexEnd = _PosToColorIndex(event->pos() + QPointF(5, 0));

    for (int i = 0; i != m_listControlPoints.size(); ++i)
    {
        if (clickedIndexBeg <= m_listControlPoints.at(i) && m_listControlPoints.at(i) <= clickedIndexEnd)
        {
            int index = m_listControlPoints.at(i);
            QColor color = QColorDialog::getColor(m_pColormap->GetColorAt(index), this, "Select Color", QColorDialog::DontUseNativeDialog);
            if (color.isValid())
            {
                m_pColormap->SetColorAt(index, color);
                update();
            }
            break;
        }
    }
}

void CKEColormapBar::_InitData()
{
    CKEColormap::InitializeColormapsFromDB();
    m_pColormap = new CKEColormap(*CKEColormap::GetColormap("rainbow"));
}

void CKEColormapBar::_DrawControlPoint(int index)
{
    if (index < 0 || 255 < index)
        return;

    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setBrush(m_pColormap->GetColorAt(index));

    QPointF pos = _ColorIndexToControlPos(index);

    QPolygonF polygon;
    polygon << pos + QPointF(-5, 10) << pos + QPointF(5, 10) << pos;
    painter.drawPolygon(polygon);
}

void CKEColormapBar::_DrawControlRange(int beg, int end)
{
    if (!(beg < end))
        return;

    QPainter painter(this);
    QPointF posBeg = _ColorIndexToControlPos(beg);
    QPointF posEnd = _ColorIndexToControlPos(end);
    painter.drawLine(posBeg.x(), posBeg.y() + 10, posEnd.x(), posEnd.y() + 10);
}

void CKEColormapBar::_DrawInterpolationMiddlePoint(int index)
{
    if (index < 0 || 255 < index)
        return;

    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setBrush(m_pColormap->GetColorAt(index));

    QPointF pos = _ColorIndexToInterpolationPos(index);

    QPolygonF polygon;
    polygon << pos + QPointF(-5, -10) << pos + QPointF(5, -10) << pos;
    painter.drawPolygon(polygon);
}

QPointF CKEColormapBar::_ColorIndexToControlPos(int index)
{
    qreal x = m_marginHorizon + index * m_singleColorBarWidth + m_singleColorBarWidth / 2;
    qreal y = height() - m_marginVerticle;
    return QPointF(x, y);
}

QPointF CKEColormapBar::_ColorIndexToInterpolationPos(int index)
{
    qreal x = m_marginHorizon + index * m_singleColorBarWidth + m_singleColorBarWidth / 2;
    qreal y = m_marginVerticle;
    return QPointF(x, y);
}

int CKEColormapBar::_PosToColorIndex(const QPointF& pos)
{
    return qRound((pos.x() - m_marginHorizon) / m_singleColorBarWidth);
}

void CKEColormapBar::_SetSingleColor(const QPointF& position)
{
    if (!m_colorBarRect.contains(position))
        return;

    int index = _PosToColorIndex(position);

    QColor color = QColorDialog::getColor(m_pColormap->GetColorAt(index), this, "Select Color", QColorDialog::DontUseNativeDialog);
    if (color.isValid())
    {
        m_pColormap->SetColorAt(index, color);
        update();
    }
}

void CKEColormapBar::_SelectControlPoint(const QPointF& position)
{
    if (!m_lowerBarRect.contains(position))
        return;

    int clickedIndexBeg = _PosToColorIndex(position + QPointF(-5, 0));
    int clickedIndexEnd = _PosToColorIndex(position + QPointF(5, 0));

    for (int i = 0; i != m_listControlPoints.size(); ++i)
    {
        if (clickedIndexBeg <= m_listControlPoints.at(i) && m_listControlPoints.at(i) <= clickedIndexEnd)
            m_selectedPointIndex = i;
    }
}

void CKEColormapBar::_SetControlPoint(const QPointF& position)
{
    if (!m_lowerBarRect.contains(position))
        return;

    m_interpolationRange = QPair<int, int>(-1, -1);

    int newControlPointIndex = _PosToColorIndex(position);

    if (m_selectedPointIndex != -1)
    {
        m_listControlPoints[m_selectedPointIndex] = newControlPointIndex;
    }
}

void CKEColormapBar::_SetMiddlePoint(const QPointF& position)
{
    if (!m_upperBarRect.contains(position))
        return;

    m_interpolationMiddlePoint = _PosToColorIndex(position);

    if (m_interpolationMiddlePoint <= 0 || 255 <= m_interpolationMiddlePoint)
        return;

    m_interpolationRange = QPair<int, int>(0, 255);

    for (int &index : m_listControlPoints)
    {
        if (m_interpolationMiddlePoint < index)
        {
            m_interpolationRange.second = qMin(m_interpolationRange.second, index);
        }
        else if (m_interpolationMiddlePoint > index)
        {
            m_interpolationRange.first = qMax(m_interpolationRange.first, index);
        }
        else
        {
            index = m_listControlPoints.contains(0) ? 255 : 0;
        }
    }

    QColor middleColor = _CalcMiddleColor(m_interpolationRange.first, m_interpolationRange.second);

    m_pColormap->SetColorAt(m_interpolationMiddlePoint, middleColor);
    _CalcLinearInterpolation(m_interpolationRange.first, m_interpolationMiddlePoint);
    _CalcLinearInterpolation(m_interpolationMiddlePoint, m_interpolationRange.second);
}

void CKEColormapBar::_CalcLinearInterpolation(int beg, int end)
{
    Q_ASSERT(0 <= beg && beg <= 255);
    Q_ASSERT(0 <= end && end <= 255);
    Q_ASSERT(beg < end);

    const QColor& colorBeg = m_pColormap->GetColorAt(beg);
    const QColor& colorEnd = m_pColormap->GetColorAt(end);

    for (int i = beg + 1; i < end; ++i)
    {
        qreal proportion = (i - beg) * 1.0 / (end - beg);
        QColor colorI;

        colorI.setRed(colorBeg.red() + qRound((colorEnd.red() - colorBeg.red()) * proportion));
        colorI.setGreen(colorBeg.green() + qRound((colorEnd.green() - colorBeg.green()) * proportion));
        colorI.setBlue(colorBeg.blue() + qRound((colorEnd.blue() - colorBeg.blue()) * proportion));

        m_pColormap->SetColorAt(i, colorI);
    }
}

QColor CKEColormapBar::_CalcMiddleColor(int beg, int end)
{
    Q_ASSERT(0 <= beg && beg <= 255);
    Q_ASSERT(0 <= end && end <= 255);
    Q_ASSERT(beg < end);

    const QColor& colorBeg = m_pColormap->GetColorAt(beg);
    const QColor& colorEnd = m_pColormap->GetColorAt(end);

    QColor color;
    color.setRed((colorBeg.red() + colorEnd.red()) / 2);
    color.setGreen((colorBeg.green() + colorEnd.green()) / 2);
    color.setBlue((colorBeg.blue() + colorEnd.blue()) / 2);

    return color;
}

void CKEColormapBar::_ResetControlPointsList()
{
    m_listControlPoints = { 0, 255 };

    m_interpolationRange = QPair<int, int>(-1, -1);
    m_interpolationMiddlePoint = -1;
}

void CKEColormapBar::_CalcCompressionToRight(int oldIndex, int newIndex)
{
    Q_ASSERT(0 <= oldIndex && oldIndex <= 255);
    Q_ASSERT(0 <= newIndex && newIndex <= 255);
    
    if (oldIndex >= newIndex)
        return;

    // compression
    int oldSize = 255 - oldIndex - 1;
    int newSize = 255 - newIndex - 1;

    QMultiMap<int, QColor> m_mapCompressed;

    for (int i = oldIndex + 1; i != 255; ++i)
    {
        m_mapCompressed.insert((i - oldIndex) * newSize / oldSize, m_pColormap->GetColorAt(i));
    }

    for (int index : m_mapCompressed.keys())
    {
        if (index == 0)
            continue;

        unsigned int r = 0, g = 0, b = 0;
        int size = m_mapCompressed.values(index).size();
        for (const QColor& color : m_mapCompressed.values(index))
        {
            r += color.red();
            g += color.green();
            b += color.blue();
        }

        QColor newColor(r / size, g / size, b / size);
        m_pColormap->SetColorAt(newIndex + index, newColor);
    }

    // stretching
    QMap<int, QColor> mapFixedIndexColor;
    
    for (int i = 0; i <= oldIndex; ++i)
    {
        mapFixedIndexColor.insert(i * newIndex / oldIndex, m_pColormap->GetColorAt(i));
    }

    for (auto iter = mapFixedIndexColor.constBegin() + 1; iter != mapFixedIndexColor.constEnd(); ++iter)
    {
        int iBeg = (iter - 1).key();
        int iEnd = iter.key();

        const QColor& colorBeg = (iter - 1).value();
        const QColor& colorEnd = iter.value();

        for (int i = iBeg; i < iEnd; ++i)
        {
            qreal proportion = (i - iBeg) * 1.0 / (iEnd - iBeg);
            QColor colorI;

            colorI.setRed(colorBeg.red() + qRound((colorEnd.red() - colorBeg.red()) * proportion));
            colorI.setGreen(colorBeg.green() + qRound((colorEnd.green() - colorBeg.green()) * proportion));
            colorI.setBlue(colorBeg.blue() + qRound((colorEnd.blue() - colorBeg.blue()) * proportion));

            m_pColormap->SetColorAt(i, colorI);
        }
    }

    for (auto iter = mapFixedIndexColor.constBegin(); iter != mapFixedIndexColor.constEnd(); ++iter)
    {
        m_pColormap->SetColorAt(iter.key(), iter.value());
    }
}

void CKEColormapBar::_CalcCompressionToLeft(int oldIndex, int newIndex)
{
    Q_ASSERT(0 <= oldIndex && oldIndex <= 255);
    Q_ASSERT(0 <= newIndex && newIndex <= 255);

    if (oldIndex <= newIndex)
        return;

    // Compression
    int oldSize = oldIndex + 1;
    int newSize = newIndex + 1;

    QMultiMap<int, QColor> m_mapCompressed;

    for (int i = 1; i < oldIndex; ++i)
    {
        m_mapCompressed.insert(i * newSize / oldSize, m_pColormap->GetColorAt(i));
    }

    for (int index : m_mapCompressed.keys())
    {
        if (index == 0)
            continue;

        unsigned int r = 0, g = 0, b = 0;
        int size = m_mapCompressed.values(index).size();
        for (const QColor& color : m_mapCompressed.values(index))
        {
            r += color.red();
            g += color.green();
            b += color.blue();
        }

        QColor newColor(r / size, g / size, b / size);
        m_pColormap->SetColorAt(index, newColor);
    }

    // stretching
    oldSize = 255 - oldIndex + 1;
    newSize = 255 - newIndex + 1;

    QMap<int, QColor> mapFixedIndexColor;

    for (int i = 0; i != oldSize; ++i)
    {
        mapFixedIndexColor.insert(newIndex + i * newSize / oldSize, m_pColormap->GetColorAt(oldIndex + i));
    }

    for (auto iter = mapFixedIndexColor.constBegin() + 1; iter != mapFixedIndexColor.constEnd(); ++iter)
    {
        int iBeg = (iter - 1).key();
        int iEnd = iter.key();

        const QColor& colorBeg = (iter - 1).value();
        const QColor& colorEnd = iter.value();

        for (int i = iBeg; i < iEnd; ++i)
        {
            qreal proportion = (i - iBeg) * 1.0 / (iEnd - iBeg);
            QColor colorI;

            colorI.setRed(colorBeg.red() + qRound((colorEnd.red() - colorBeg.red()) * proportion));
            colorI.setGreen(colorBeg.green() + qRound((colorEnd.green() - colorBeg.green()) * proportion));
            colorI.setBlue(colorBeg.blue() + qRound((colorEnd.blue() - colorBeg.blue()) * proportion));

            m_pColormap->SetColorAt(i, colorI);
        }
    }

    for (auto iter = mapFixedIndexColor.constBegin(); iter != mapFixedIndexColor.constEnd(); ++iter)
    {
        m_pColormap->SetColorAt(iter.key(), iter.value());
    }
}

bool CKEColormapBar::_CheckBoundingRange(const QPointF& position)
{
    if (!m_lowerBarRect.contains(position))
        return false;

    int currentIndex = _PosToColorIndex(position);

    int boundingIndex = m_selectedPointIndex == 0 ? m_listControlPoints.at(1) : m_listControlPoints.at(0);

    if (boundingIndex > m_compressionIndex)
    {
        return currentIndex < boundingIndex;
    }
    else
    {
        return currentIndex > boundingIndex;
    }
}