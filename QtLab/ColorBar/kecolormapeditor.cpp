#include "kecolormapeditor.h"

#include <QPainter>
#include <QResizeEvent>

#include "kecolormap.h"

CKEColormapEditor::CKEColormapEditor(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(QSize(200, 320));
    setMouseTracking(true);
}

CKEColormapEditor::~CKEColormapEditor()
{

}

void CKEColormapEditor::slotReset()
{
    if (!m_pColormap || !m_pTemplateColormap)
        return;

    for (int i = 0; i != 256; ++i)
    {
        m_pColormap->SetColorAt(i, m_pTemplateColormap->GetColorAt(i));
    }

    m_pColormap->ResetTurningPoint();

    update();
}

void CKEColormapEditor::slotFlip()
{
    if (!m_pColormap)
        return;

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

        for (int i = 0; i != 256; ++i)
        {
            m_pColormap->SetColorAt(i, m_pTemplateColormap->GetColorAt(i));
        }

        m_pColormap->ResetTurningPoint();
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

    m_singleColorBarWidth = m_colorBarRect.width() / 256;

    QPainter painter(this);
    painter.save();
    painter.drawTiledPixmap(m_alphaRect, tile);

    for (int i = 0; i != 256; ++i)
    {
        painter.setPen(m_pColormap->GetColorAt(i));
        painter.setBrush(m_pColormap->GetColorAt(i));
        painter.drawRect(i * m_singleColorBarWidth, m_colorBarRect.y(), m_singleColorBarWidth, m_colorBarRect.height());
    }

    for (auto i : m_pColormap->GetTurningPointIndex())
    {
        _DrawControlPoint(i);
    }

    painter.restore();
}

void CKEColormapEditor::resizeEvent(QResizeEvent* event)
{
    qreal alphaRectHeight = (event->size().height() - m_controlRectHeight) * 0.1;
    qreal colorBarHeight = (event->size().height() - m_controlRectHeight) * 0.9;
    m_alphaRect.setRect(0, 0, event->size().width(), alphaRectHeight);
    m_colorBarRect.setRect(0, alphaRectHeight, event->size().width(), colorBarHeight);
    m_controlRect.setRect(0, event->size().height() - m_controlRectHeight, event->size().width(), m_controlRectHeight);
}

void CKEColormapEditor::mouseMoveEvent(QMouseEvent* event)
{
    if (m_colorBarRect.contains(event->pos()))
    {
        int curIndex = _PosToColorIndex(event->pos());
        QRgb curRgb = m_pColormap->GetColorAt(curIndex).rgb();
        emit ShowMsg("Index: " + QString::number(curIndex) + QString("\tColor: %1,%2,%3").arg(qRed(curRgb)).arg(qGreen(curRgb)).arg(qBlue(curRgb)));
    }
    else if (m_controlRect.contains(event->pos()))
    {
        int controlIndexBeg = _PosToColorIndex(event->pos() + QPointF(-5, 0));
        int controlIndexEnd = _PosToColorIndex(event->pos() + QPointF( 5, 0));

        for (auto index : m_pColormap->GetTurningPointIndex())
        {
            if (controlIndexBeg <= index && index <= controlIndexEnd)
            {
                QRgb curRgb = m_pColormap->GetColorAt(index).rgb();
                emit ShowMsg("Control point: " + QString::number(index) + QString("\tColor: %1,%2,%3").arg(qRed(curRgb)).arg(qGreen(curRgb)).arg(qBlue(curRgb)));
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
    return qRound(pos.x() / m_singleColorBarWidth);
}

void CKEColormapEditor::_DrawControlPoint(int index)
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