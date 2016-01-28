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
    _Init();
}

CKEColormapBar::~CKEColormapBar()
{

}

void CKEColormapBar::paintEvent(QPaintEvent*)
{
    m_singleColorBarWidth = m_colorBarRect.width() / m_nColor;

    QPainter painter(this);

    for (int i = 0; i != m_nColor; ++i)
    {
        painter.setPen(m_colormap->GetColorAt(i));
        painter.setBrush(m_colormap->GetColorAt(i));
        painter.drawRect(i * m_singleColorBarWidth + m_marginHorizon, m_marginVerticle, m_singleColorBarWidth, m_colorBarRect.height());
    }

    for (auto i : m_mapCPRect.keys())
    {
        _DrawControlPoint(i);
    }

    if (m_interpolationIndex != -1)
    {
        _DrawInterpolationIndex(m_interpolationIndex);
    }
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
    if (event->button() != Qt::LeftButton)
        return;

    int clickedIndex = _ControlPointIndexAtPos(event->pos());

    if (m_highlightIndex == -1) // no highlight.
    {
        if (clickedIndex == -1)
        {
            m_interpolationIndex = _PosToColorIndex(event->pos());
        }
        else
        {
            m_highlightIndex = clickedIndex;
        }
    }
    else
    {
        if (clickedIndex == -1)
        {
            int index = _PosToColorIndex(event->pos());
            QPointF pos = _ColorIndexToControlPos(index);
            m_mapCPRect.insert(index, QRectF(pos + QPointF(-5, 0), QSize(10, 10)));

            if (m_highlightIndex != 0 && m_highlightIndex != 255)
                m_mapCPRect.remove(m_highlightIndex);

            m_highlightIndex = -1;
        }
        else
        {
            m_highlightIndex = clickedIndex;
        }
    }

    update();
}

void CKEColormapBar::_Init()
{
    m_mapCPRect.insert(0, QRectF());
    m_mapCPRect.insert(255, QRectF());
} 

void CKEColormapBar::_InitData()
{
    CKEColormap::InitializeColormapsFromDB();
    m_colormap = CKEColormap::GetColormap("rainbow");
}

void CKEColormapBar::_DrawControlPoint(int index)
{
    QPainter painter(this);

    painter.setPen(m_colormap->GetColorAt(index));
    painter.setBrush(m_colormap->GetColorAt(index));

    if (m_highlightIndex == index)
        painter.setPen(QPen(Qt::black, 1.5));

    QPointF pos = _ColorIndexToControlPos(index);
    m_mapCPRect[index] = QRectF(pos + QPointF(-5, 0), QSize(10, 10));

    QPolygonF polygon;
    polygon << pos + QPointF(-5, 10) << pos + QPointF(5, 10) << pos;
    painter.drawPolygon(polygon);
}

void CKEColormapBar::_DrawInterpolationIndex(int index)
{
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setBrush(m_colormap->GetColorAt(index));

    QPointF pos = _ColorIndexToInterpolationPos(index);
    m_rectInterpolation = QRectF(pos + QPointF(-5, -10), QSize(10, 10));

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

int CKEColormapBar::_ControlPointIndexAtPos(QPointF pos)
{
    for (auto iter = m_mapCPRect.constBegin(); iter != m_mapCPRect.constEnd(); ++iter)
    {
        if (iter.value().contains(pos))
            return iter.key();
    }

    return -1;
}

void CKEColormapBar::_ChangeSingleColor(const QPointF& position)
{
    QColor color = QColorDialog::getColor(m_colormap->GetColorAt(m_highlightIndex), this, "Select Color", QColorDialog::DontUseNativeDialog);
    if (color.isValid())
    {
        m_colormap->SetColorAt(m_highlightIndex, color);
        update();
    }
}

void CKEColormapBar::_ColorInterpolation()
{
    QColor color = QColorDialog::getColor(m_colormap->GetColorAt(m_interpolationIndex), this, "Select Color", QColorDialog::DontUseNativeDialog);
    if (color.isValid())
    {
        m_colormap->SetColorAt(m_interpolationIndex, color);

        QList<int> listCP = m_mapCPRect.keys();
        auto lower = std::lower_bound(listCP.constBegin(), listCP.constEnd(), m_interpolationIndex);
        auto upper = std::upper_bound(listCP.constBegin(), listCP.constEnd(), m_interpolationIndex);

        --lower;
        int indexBeg = *lower;
        int indexEnd = *upper;

        QColor colorBeg = m_colormap->GetColorAt(indexBeg);
        QColor colorEnd = m_colormap->GetColorAt(indexEnd);

        for (int i = indexBeg + 1; i < m_interpolationIndex; ++i)
        {
            qreal proportion = (i - indexBeg) * 1.0 / (m_interpolationIndex - indexBeg);
            QColor colorI;

            colorI.setRed(colorBeg.red() + qRound((color.red() - colorBeg.red()) * proportion));
            colorI.setGreen(colorBeg.green() + qRound((color.green() - colorBeg.green()) * proportion));
            colorI.setBlue(colorBeg.blue() + qRound((color.blue() - colorBeg.blue()) * proportion));

            m_colormap->SetColorAt(i, colorI);
        }

        for (int i = m_interpolationIndex + 1; i < indexEnd; ++i)
        {
            qreal proportion = (i - m_interpolationIndex) * 1.0 / (indexEnd - m_interpolationIndex);
            QColor colorI;

            colorI.setRed(color.red() + qRound((colorEnd.red() - color.red()) * proportion));
            colorI.setGreen(color.green() + qRound((colorEnd.green() - color.green()) * proportion));
            colorI.setBlue(color.blue() + qRound((colorEnd.blue() - color.blue()) * proportion));

            m_colormap->SetColorAt(i, colorI);
        }

        update();
    }
}