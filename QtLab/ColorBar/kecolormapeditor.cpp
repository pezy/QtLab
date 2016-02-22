#include "kecolormapeditor.h"

#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <qmath.h>

#include "kecolormap.h"

CKEColormapEditor::CKEColormapEditor(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(QSize(200, 320));
    setMouseTracking(true);
}

CKEColormapEditor::~CKEColormapEditor()
{
    if (m_pColormap)
    {
        delete m_pColormap;
        m_pColormap = nullptr;
    }
}

void CKEColormapEditor::slotReset()
{
    if (!m_pColormap || !m_pTemplateColormap)
        return;

    *m_pColormap = *m_pTemplateColormap;

    update();
}

void CKEColormapEditor::slotFlip()
{
    if (!m_pColormap)
        return;

    m_pColormap->Flip();

    update();
}

void CKEColormapEditor::slotTemplateChanged(const QString& templateName)
{
    if (!m_pColormap)
    {
        m_pColormap = new CKEColormap(*CKEColormap::GetColormap(templateName));
    }
    else
    {
        m_pTemplateColormap = CKEColormap::GetColormap(templateName);
        if (!m_pTemplateColormap)
            return;

        *m_pColormap = *m_pTemplateColormap;
    }

    update();
}

void CKEColormapEditor::paintEvent(QPaintEvent* event)
{
    if (!m_pColormap)
        return;

    QPixmap tile = QPixmap(16, 16);
    tile.fill(Qt::white);
    QPainter pt(&tile);
    QColor color(230, 230, 230);
    pt.fillRect(0, 0, 8, 8, color);
    pt.fillRect(8, 8, 8, 8, color);
    pt.end();

    m_singleColorBarWidth = m_colorBarRect.width() / m_pColormap->GetColorNum();

    QPainter painter(this);
    painter.save();
    painter.drawTiledPixmap(m_colorBarRect, tile);

    for (int i = 0; i != m_pColormap->GetColorNum(); ++i)
    {
        painter.setPen(m_pColormap->GetColorAt(i));
        painter.setBrush(m_pColormap->GetColorAt(i));
        painter.drawRect(i * m_singleColorBarWidth, m_colorBarRect.y(), m_singleColorBarWidth, m_colorBarRect.height());
    }

    for (auto i : m_pColormap->GetControlPointsIndex())
    {
        _DrawControlPoint(i);
    }

    painter.restore();
}

void CKEColormapEditor::resizeEvent(QResizeEvent* event)
{
    m_colorBarRect.setRect(0, 0, event->size().width(), event->size().height() - m_controlRectHeight);
    m_controlRect.setRect(0, event->size().height() - m_controlRectHeight, event->size().width(), m_controlRectHeight);
}

void CKEColormapEditor::mouseMoveEvent(QMouseEvent* event)
{
    if (qFuzzyCompare(m_singleColorBarWidth, 0.0))
        return;

    if (m_colorBarRect.contains(event->pos()))
    {
        int curIndex = _PosToColorIndex(event->pos());
        QRgb curRgb = m_pColormap->GetColorAt(curIndex).rgba();
        emit ShowMsg("Index: " + QString::number(curIndex) + QString("\tColor: %1,%2,%3,%4").arg(qRed(curRgb)).arg(qGreen(curRgb)).arg(qBlue(curRgb)).arg(qAlpha(curRgb)));
    }
    else if (m_controlRect.contains(event->pos()))
    {
        int controlIndexBeg = _PosToColorIndex(event->pos() + QPointF(-5, 0));
        int controlIndexEnd = _PosToColorIndex(event->pos() + QPointF( 5, 0));

        for (auto index : m_pColormap->GetControlPointsIndex())
        {
            if (controlIndexBeg <= index && index <= controlIndexEnd)
            {
                QRgb curRgb = m_pColormap->GetColorAt(index).rgba();
                emit ShowMsg("Control point: " + QString::number(index) + QString("\tColor: %1,%2,%3,%4").arg(qRed(curRgb)).arg(qGreen(curRgb)).arg(qBlue(curRgb)).arg(qAlpha(curRgb)));
            }
        }
    }
    else
    {
        emit ShowMsg("");
    }
}

QPointF CKEColormapEditor::_ColorIndexToControlPos(int index)
{
    qreal x = index * m_singleColorBarWidth + m_singleColorBarWidth / 2;
    qreal y = height() - m_controlRectHeight;
    return QPointF(x, y);
}

int CKEColormapEditor::_PosToColorIndex(const QPointF& pos)
{
    return qFloor(pos.x() / m_singleColorBarWidth);
}

void CKEColormapEditor::_DrawControlPoint(int index)
{
    if (index < 0 || m_pColormap->GetColorNum() <= index)
        return;

    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setBrush(m_pColormap->GetColorAt(index));

    QPointF pos = _ColorIndexToControlPos(index);

    QPolygonF polygon;
    polygon << pos + QPointF(-5, 10) << pos + QPointF(5, 10) << pos;
    painter.drawPolygon(polygon);
}