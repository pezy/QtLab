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
    m_mapGeologicMask.clear();
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
    m_mapGeologicMask.clear();

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

    m_mapGeologicMask.clear();

    update();
}

void CKEColormapEditor::slotAddControlPoint()
{
    
    
    m_pColormap->SetControlPointRgb(m_selectedColorIndex, m_pColormap->GetColorAt(m_selectedColorIndex).rgb());

    update();
}

void CKEColormapEditor::slotAddGeologicMask()
{
    Q_ASSERT(m_selectedColorIndex != -1);

    m_mapGeologicMask.insert(m_selectedColorIndex, qRgb(255, 0, 255));
    m_pColormap->UpdateGeologicMask(m_mapGeologicMask);

    update();
}

void CKEColormapEditor::slotDeleteControlPointOrMask()
{
    if (m_mapGeologicMask.contains(m_selectedColorIndex))
    {
        m_mapGeologicMask.remove(m_selectedColorIndex);
        m_pColormap->UpdateGeologicMask(m_mapGeologicMask);
    }
    else
    {
        m_pColormap->DeleteControlPoint(m_selectedColorIndex);
    }

    update();
}

void CKEColormapEditor::slotAllOpaque()
{
	m_pColormap->ClearAlpha();
	update();
}

void CKEColormapEditor::slotDeleteOpacityPoint()
{
	m_pColormap->DeleteOpacityPoint(m_selectedColorIndex);
	update();
}

void CKEColormapEditor::slotAddOpacityPoint()
{
	m_pColormap->SetAlpha(m_selectedColorIndex, static_cast<uchar>(m_selectedAlpha));
	update();
}

void CKEColormapEditor::slotSetOpacityFreeDrawing(bool bChecked)
{
	m_bFreeDrawing = bChecked;
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

	_DrawOpacityPoint();
    _DrawGeologicMask();

    painter.restore();
}

void CKEColormapEditor::resizeEvent(QResizeEvent* event)
{
    m_colorBarRect.setRect(0, 0, event->size().width(), event->size().height() - m_controlRectHeight);
    m_controlRect.setRect(0, event->size().height() - m_controlRectHeight, event->size().width(), m_controlRectHeight);
}

void CKEColormapEditor::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bGeologicMaskChanged)
    {
        int newIndex = _PosToColorIndex(event->pos());
		if (m_listFixedMask.contains(newIndex) || newIndex == m_selectedColorIndex || newIndex == -1)
            return;

        m_mapGeologicMask.insert(newIndex, m_selectedMaskRgb);
        m_mapGeologicMask.remove(m_selectedColorIndex);
        m_pColormap->UpdateGeologicMask(m_mapGeologicMask);
        m_selectedColorIndex = newIndex;

        update();
        return;
    }
    
    if (m_bControlIndexChanged)
    {
        int newIndex = _PosToColorIndex(event->pos());
		if (m_listFixedControlPoints.contains(newIndex) || newIndex == -1)
            return;

        m_pColormap->UpdateControlPoint(m_selectedColorIndex, newIndex, m_selectedControlPointRgb);
        m_selectedColorIndex = newIndex;

        update();
        return;
    }

	if (m_bDrawingStart)
	{
		int newIndex = _PosToColorIndex(event->pos());
		int newAlpha = _PosToAlpha(event->pos());

		if (newIndex == -1 || newAlpha == -1)
			return;

		int indexBeg = qMin(m_selectedColorIndex, newIndex);
		int indexEnd = qMax(m_selectedColorIndex, newIndex);

		const QMap<int, uchar>& mapAlpha = m_pColormap->GetAlphaMap();
		for (int i = indexBeg + 1; i < indexEnd; ++i)
		{
			if (mapAlpha.contains(i))
				m_pColormap->DeleteOpacityPoint(i);
		}

		m_pColormap->SetAlpha(newIndex, newAlpha);
		m_selectedColorIndex = newIndex;

		update();
		return;
	}

    if (m_colorBarRect.contains(event->pos()))
    {
        _ShowColorInfo(event->pos());
    }
    else if (m_controlRect.contains(event->pos()))
    {
        int possibleControlIndex;
        if (_IsInControlPoint(event->pos(), possibleControlIndex))
        {
            const QColor& color = m_pColormap->GetColorAt(possibleControlIndex);
			emit ShowMsg(QString("Control point: %5\nColor: %1,%2,%3,%4\nAlpha: (null)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()).arg(possibleControlIndex));
        }
    }
}

void CKEColormapEditor::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        _ShowMenu(event->pos(), event->globalPos());
    }
    else if (event->button() == Qt::LeftButton)
    {
        _SelectControlPointOrMask(event->pos());
		_BeginFreeDrawing(event->pos());
    }
}

void CKEColormapEditor::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _SetControlPointColor(event->pos());
    }
}

void CKEColormapEditor::mouseReleaseEvent(QMouseEvent* event)
{
    m_selectedColorIndex = -1;
    m_bControlIndexChanged = false;
    m_selectedControlPointRgb = 0u;
    m_listFixedControlPoints.clear();
    m_bGeologicMaskChanged = false;
    m_listFixedMask.clear();
	m_bDrawingStart = false;
}

QPointF CKEColormapEditor::_ColorIndexToControlPos(int index)
{
    if (qFuzzyCompare(m_singleColorBarWidth, 0.0))
        return QPointF();

    qreal x = index * m_singleColorBarWidth + m_singleColorBarWidth / 2;
    qreal y = height() - m_controlRectHeight;
    return QPointF(x, y);
}

QPointF CKEColormapEditor::_IndexAlphaToOpacityPos(int index, int alpha)
{
	if (qFuzzyCompare(m_singleColorBarWidth, 0.0))
		return QPointF();

	qreal x = index * m_singleColorBarWidth + m_singleColorBarWidth / 2;
	qreal y = (1 - alpha * 1.0 / 255) * m_colorBarRect.height();
	return QPointF(x, y);
}

int CKEColormapEditor::_PosToColorIndex(const QPointF& pos)
{
	if (m_singleColorBarWidth == 0.0 || pos.x() >= m_colorBarRect.width() || pos.x() < 0)
        return -1;
    else
        return qFloor(pos.x() / m_singleColorBarWidth);
}

int CKEColormapEditor::_PosToAlpha(const QPointF& pos)
{
	if (pos.y() > m_colorBarRect.height() || pos.y() < 0)
		return -1;
	else
		return qRound((1 - pos.y() / m_colorBarRect.height()) * 255);
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

void CKEColormapEditor::_DrawGeologicMask()
{
    QPainter painter(this);
    painter.setPen(Qt::black);

    for (auto iter = m_mapGeologicMask.cbegin(); iter != m_mapGeologicMask.cend(); ++iter)
    {
        painter.save();
        painter.setBrush(QColor(iter.value()));

        QPointF pos = _ColorIndexToControlPos(iter.key());
        if (pos.isNull())
            return;

        painter.drawRect(QRectF(pos.x() - 5, pos.y(), 10, 10));
        painter.restore();
    }
}

void CKEColormapEditor::_DrawOpacityPoint()
{
	QPolygonF alphaPoint;
	const QMap<int, uchar>& mapAlpah = m_pColormap->GetAlphaMap();
	for (auto iter = mapAlpah.cbegin(); iter != mapAlpah.cend(); ++iter)
	{
		QPointF pos = _IndexAlphaToOpacityPos(iter.key(), iter.value());
		if (pos.isNull())
			return;

		alphaPoint.push_back(pos);
	}

	QPainter painter(this);

	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(Qt::white, 3));
	painter.drawPolyline(alphaPoint);
	painter.setPen(QColor(153, 153, 153));
	painter.drawPolyline(alphaPoint);
	painter.restore();

	painter.setPen(Qt::black);
	painter.setBrush(Qt::yellow);

	for (auto pos : alphaPoint)
	{
		painter.drawRect(QRectF(pos.x() - 4, pos.y() - 4, 8, 8));
	}
}

void CKEColormapEditor::_OpenControlMenu(QMenu& menu, const QPoint& pos)
{
    QAction *pAddControlPointAction = menu.addAction("Add control point", this, SLOT(slotAddControlPoint()));
    QAction *pAddGeologicMaskAction = menu.addAction("Add geologic mask", this, SLOT(slotAddGeologicMask()));
    QAction *pDeleteAction = menu.addAction("Delete control point or mask", this, SLOT(slotDeleteControlPointOrMask()));

    m_selectedColorIndex = _PosToColorIndex(pos);

	if (_IsInControlPoint(pos, m_selectedColorIndex) || _IsInGeologicMask(pos, m_selectedColorIndex))
    {
        pAddControlPointAction->setDisabled(true);
        pDeleteAction->setDisabled(false);
    }
    else
    {
        pAddControlPointAction->setDisabled(false);
        pDeleteAction->setDisabled(true);
    }
}

void CKEColormapEditor::_OpenOpacityMenu(QMenu& menu, const QPoint& pos)
{
	QAction *pAddOpacityPointAction = menu.addAction("Add opacity point", this, SLOT(slotAddOpacityPoint()));
	QAction *pDelOpacityPointAction = menu.addAction("Delete opacity point", this, SLOT(slotDeleteOpacityPoint()));
	QAction *pAllOpaqueAction = menu.addAction("All opaque", this, SLOT(slotAllOpaque()));

	m_selectedColorIndex = _PosToColorIndex(pos);
	m_selectedAlpha = _PosToAlpha(pos);
	
	if (_IsInOpacityPoint(pos, m_selectedColorIndex, m_selectedAlpha))
	{
		pAddOpacityPointAction->setDisabled(true);
		pDelOpacityPointAction->setDisabled(false);
	}
	else
	{
		pAddOpacityPointAction->setDisabled(false);
		pDelOpacityPointAction->setDisabled(true);
	}
}

bool CKEColormapEditor::_IsInControlPoint(const QPoint& pos, int& controlIndex)
{
    int indexBeg = _PosToColorIndex(pos + QPointF(-5, 0));
    int indexEnd = _PosToColorIndex(pos + QPointF(5, 0));

    if (indexBeg == -1 || indexEnd == -1)
        return false;

    const std::list<int>& controlPointIndices = m_pColormap->GetControlPointsIndex().toStdList();
    for (auto iter = controlPointIndices.rbegin(); iter != controlPointIndices.rend(); ++iter)
    {
        if (indexBeg <= *iter && *iter <= indexEnd)
        {
            controlIndex = *iter;
            return true;
        }
    }

    return false;
}

bool CKEColormapEditor::_IsInGeologicMask(const QPoint& pos, int& maskIndex)
{
    int indexBeg = _PosToColorIndex(pos + QPointF(-5, 0));
    int indexEnd = _PosToColorIndex(pos + QPointF(5, 0));

    if (indexBeg == -1 || indexEnd == -1)
        return false;

    const std::list<int>& maskIndices = m_mapGeologicMask.keys().toStdList();
    for (auto iter = maskIndices.rbegin(); iter != maskIndices.rend(); ++iter)
    {
        if (indexBeg <= *iter && *iter <= indexEnd)
        {
            maskIndex = *iter;
            return true;
        }
    }

    return false;
}

bool CKEColormapEditor::_IsInOpacityPoint(const QPoint& pos, int& controlIndex, int& alpha)
{
	int indexBeg = _PosToColorIndex(pos + QPointF(-4, 0));
	int indexEnd = _PosToColorIndex(pos + QPointF( 4, 0));
	int alphaBeg = _PosToAlpha(pos + QPointF(0,  4));
	int alphaEnd = _PosToAlpha(pos + QPointF(0, -4));

	if (indexBeg == -1 || indexEnd == -1)
		return false;

	const std::map<int, uchar>& mapAlpha = m_pColormap->GetAlphaMap().toStdMap();
	for (auto iter = mapAlpha.crbegin(); iter != mapAlpha.crend(); ++iter)
	{
		if (indexBeg <= iter->first && iter->first <= indexEnd && alphaBeg <= iter->second && iter->second <= alphaEnd)
		{
			controlIndex = iter->first;
			alpha = iter->second;
			return true;
		}
	}

	return false;
}

void CKEColormapEditor::_ShowMenu(const QPoint& position, const QPoint& cursorPos)
{
    QMenu menu(this);
	if (m_controlRect.contains(position))
	{
		_OpenControlMenu(menu, position);
	}
	else if (m_colorBarRect.contains(position))
	{
		_OpenOpacityMenu(menu, position);
	}
    menu.exec(cursorPos);
}

void CKEColormapEditor::_SetControlPointColor(const QPoint& position)
{
    if (!m_controlRect.contains(position))
        return;

    int possibleControlIndex;
    if (!_IsInControlPoint(position, possibleControlIndex))
        return;

    QColor color = QColorDialog::getColor(m_pColormap->GetColorAt(possibleControlIndex), this, "Select Color", QColorDialog::DontUseNativeDialog);
    if (color.isValid())
    {
        m_pColormap->SetControlPointRgb(possibleControlIndex, color.rgb());
        update();
    }
}

void CKEColormapEditor::_ShowColorInfo(const QPoint& position)
{
	int currIndex = _PosToColorIndex(position);
	int currAlpha = _PosToAlpha(position);
	if (currIndex == -1)
		return;

	if (_IsInOpacityPoint(position, currIndex, currAlpha))
	{
		const QColor& color = m_pColormap->GetColorAt(currIndex);
		emit ShowMsg(QString("Opacity point: %1\nColor: %2,%3,%4,%5\nAlpha: %6").arg(currIndex).arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()).arg(currAlpha));

	}
	else
	{
		const QColor& color = m_pColormap->GetColorAt(currIndex);
		emit ShowMsg(QString("Index: %1\nColor: %2,%3,%4,%5\nAlpha: %6").arg(currIndex).arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()).arg(currAlpha));

	}
}

void CKEColormapEditor::_SelectControlPointOrMask(const QPoint& pos)
{
    if (!m_controlRect.contains(pos))
        return;

    if (_IsInGeologicMask(pos, m_selectedColorIndex))
    {
        m_bGeologicMaskChanged = true;
        m_selectedMaskRgb = m_mapGeologicMask.value(m_selectedColorIndex);
        m_listFixedMask = m_mapGeologicMask.keys();
        m_listFixedMask.removeOne(m_selectedColorIndex);
    }
    else if (_IsInControlPoint(pos, m_selectedColorIndex))
    {
        m_bControlIndexChanged = true;
        m_selectedControlPointRgb = m_pColormap->GetRgbAt(m_selectedColorIndex);
        m_listFixedControlPoints = m_pColormap->GetControlPointsIndex();
        m_listFixedControlPoints.removeOne(m_selectedColorIndex);
    }
}

void CKEColormapEditor::_BeginFreeDrawing(const QPoint& pos)
{
	if (!m_colorBarRect.contains(pos) || !m_bFreeDrawing)
		return;

	m_bDrawingStart = true;
	m_selectedColorIndex = _PosToColorIndex(pos);
}