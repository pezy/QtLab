#include "kecolormapeditor.h"

#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <qmath.h>
#include <QColorDialog>

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
    m_pTemplateColormap = CKEColormap::GetColormap(templateName);
    
    if (!m_pTemplateColormap)
        return;

    if (!m_pColormap)
    {
        m_pColormap = new CKEColormap(*m_pTemplateColormap);
    }
    else
    {
        *m_pColormap = *m_pTemplateColormap;
    }

    update();
}

void CKEColormapEditor::slotAddControlPoint()
{
    if (m_currentControlPointIndex == -1 && m_clickedColorIndex != -1)
    {
        m_pColormap->SetControlPointRgb(m_clickedColorIndex, m_pColormap->GetColorAt(m_clickedColorIndex).rgb());
    }

    update();
}

void CKEColormapEditor::slotAddGeologicMask()
{

}

void CKEColormapEditor::slotDeleteControlPointOrMask()
{
    if (m_currentControlPointIndex != -1)
    {
        m_pColormap->DeleteControlPoint(m_currentControlPointIndex);
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
    if (m_colorBarRect.contains(event->pos()))
    {
        _ShowColorInfo(event->pos());
    }
    else if (m_controlRect.contains(event->pos()))
    {
        if (_IsInControlPoint(event->pos()))
        {
            const QColor& color = m_pColormap->GetColorAt(m_currentControlPointIndex);
            emit ShowMsg("Control point: " + QString::number(m_currentControlPointIndex) + QString("\tColor: %1,%2,%3,%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
        }
    }
    else
    {
        emit ShowMsg("");
    }
}

void CKEColormapEditor::mousePressEvent(QMouseEvent* event)
{
    m_clickedColorIndex = _PosToColorIndex(event->pos());

    if (m_controlRect.contains(event->pos()) && event->button() == Qt::RightButton)
    {
        QMenu menu(this);
        _IsInControlPoint(event->pos());
        _UpdateMenuState(menu);
        menu.exec(event->globalPos());
    }
}

void CKEColormapEditor::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _SetSingleColor(event->pos());
    }
}

QPointF CKEColormapEditor::_ColorIndexToControlPos(int index)
{
    if (qFuzzyCompare(m_singleColorBarWidth, 0.0))
        return QPointF();

    qreal x = index * m_singleColorBarWidth + m_singleColorBarWidth / 2;
    qreal y = height() - m_controlRectHeight;
    return QPointF(x, y);
}

int CKEColormapEditor::_PosToColorIndex(const QPointF& pos)
{
    if (qFuzzyCompare(m_singleColorBarWidth, 0.0))
        return -1;
    else
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
    if (pos.isNull())
        return;

    QPolygonF polygon;
    polygon << pos + QPointF(-5, 10) << pos + QPointF(5, 10) << pos;
    painter.drawPolygon(polygon);
}

void CKEColormapEditor::_UpdateMenuState(QMenu& menu)
{
    QAction *pAddControlPointAction = menu.addAction("Add control point", this, SLOT(slotAddControlPoint()));
    QAction *pAddGeologicMaskAction = menu.addAction("Add geologic mask", this, SLOT(slotAddGeologicMask()));
    QAction *pDeleteAction = menu.addAction("Delete control point or mask", this, SLOT(slotDeleteControlPointOrMask()));

    if (m_currentControlPointIndex == -1)
    {
        pAddControlPointAction->setDisabled(false);
        pDeleteAction->setDisabled(true);
    }
    else
    {
        pAddControlPointAction->setDisabled(true);
        pDeleteAction->setDisabled(false);
    }
}

bool CKEColormapEditor::_IsInControlPoint(const QPoint& pos)
{
    m_currentControlPointIndex = -1;

    int indexBeg = _PosToColorIndex(pos + QPointF(-5, 0));
    int indexEnd = _PosToColorIndex(pos + QPointF(5, 0));

    if (indexBeg == -1 || indexEnd == -1)
        return false;

    for (auto index : m_pColormap->GetControlPointsIndex())
    {
        if (indexBeg <= index && index <= indexEnd)
        {
            m_currentControlPointIndex = index;
        }
    }

    return m_currentControlPointIndex != -1;
}

void CKEColormapEditor::_SetSingleColor(const QPoint& position)
{
    if (!m_controlRect.contains(position))
        return;

    if (!_IsInControlPoint(position))
        return;

    QColor color = QColorDialog::getColor(m_pColormap->GetColorAt(m_currentControlPointIndex), this, "Select Color", QColorDialog::DontUseNativeDialog);
    if (color.isValid())
    {
        m_pColormap->SetControlPointRgb(m_currentControlPointIndex, color.rgb());
        update();
    }
}

void CKEColormapEditor::_ShowColorInfo(const QPointF& position)
{
    int currIndex = _PosToColorIndex(position);
    if (currIndex == -1)
        return;

    const QColor& color = m_pColormap->GetColorAt(currIndex);
    emit ShowMsg("Index: " + QString::number(currIndex) + QString("\tColor: %1,%2,%3,%4").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}