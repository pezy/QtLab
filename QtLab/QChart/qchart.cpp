#include "qchart.h"
#include <QDebug>
#include <QPainter>
#include <QFontMetrics>
#include <QWheelEvent>

#include "qselfadjustingaxis.h"

QChart::QChart(QWidget *parent)
    : QWidget(parent), titleHeight(50), tipHeight(50)
{
}

QChart::~QChart()
{
}

void QChart::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	/*!
	 * Set main rect
	 */

	QRect rect = this->geometry();
	rect.moveTopLeft(QPoint(0, 0));
	rect.adjust(10, 10, -11, -11); /**< set aside for edge. */

	if (_axis == nullptr) return;
	else if (!_axis->hasRect()) _axis->setRect(QRect(rect.left(), rect.top()+titleHeight, rect.width(), rect.height() - titleHeight - tipHeight));

	/*!
	 * Draw the border
	 */
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(Qt::white));
	painter.drawRoundRect(rect, 10, 10);

	/*!
	 * Draw the Title
	 */
	painter.setPen(Qt::SolidLine);
	painter.setFont(QFont("Microsoft YaHei UI", 12, QFont::DemiBold)); /**< Use YaHei Font */
	painter.drawText(QRect(rect.left(), rect.top(), rect.width(), titleHeight), Qt::AlignCenter, title());
	painter.translate(_axis->basePoint());

	/*!
	 * Draw the tips
	 */
	painter.setFont(QFont("Microsoft YaHei UI", 8));
	QFontMetrics tipsMetrics = painter.fontMetrics();
	int tipsLen = 0;
	const int sideLen = 5;
	// get tip-bar size and left bearing
	for (int i = 0; i != _series->size(); ++i)
		tipsLen += tipsMetrics.width(_series->at(i)->name()) + sideLen + 2*QSelfAdjustingAxis::scaleLen;
	int leftTipsBearing = (_axis->width() - tipsLen) / 2;
	// draw
	for (int i = 0; i != _series->size(); ++i)
	{
		int strLen = tipsMetrics.width(_series->at(i)->name());
		painter.setBrush(_series->at(i)->color());
		// box
		painter.drawRect(leftTipsBearing, tipHeight, sideLen, sideLen);
		leftTipsBearing += sideLen + QSelfAdjustingAxis::scaleLen;
		// text
		painter.drawText(leftTipsBearing, tipHeight + sideLen + 1, _series->at(i)->name());
		leftTipsBearing += strLen + QSelfAdjustingAxis::scaleLen;
	}

	/*!
	 * Draw the coordinate and histogram
	 */
	painter.setFont(_axis->font());
	int histogramSize = _series->size()*sideLen;
	float histogramEdge = (_axis->deltaX() - histogramSize)/2;

	painter.drawLine(0, 0, _axis->width(), 0);
	for (int i = 0; i != _axis->categories().size(); ++i)
	{
		// histogram
		for (int j = 0; j != _series->size(); ++j)
		{
			painter.setBrush(_series->at(j)->color());
			painter.setPen(Qt::NoPen);
			float tips = _axis->valueToTick(_series->at(j)->at(i));
			painter.drawRect(_axis->deltaX()*i + histogramEdge + 5*j, -tips, 4, tips);
		}
		painter.setPen(Qt::SolidLine);

		int strLen = _axis->fontMetrics().width(_axis->categories().at(i));
		// scale
		painter.drawLine(_axis->deltaX()*(i+1), 0, _axis->deltaX()*(i+1), QSelfAdjustingAxis::scaleLen);
		// text
		painter.drawText(_axis->deltaX()*i + (_axis->deltaX()-strLen)/2 , QSelfAdjustingAxis::scaleLen + _axis->fontMetrics().height(), _axis->categories().at(i));
	}

	painter.drawLine(0, 0, 0, -_axis->height());
	for (int i = 0; i != _axis->values().size(); ++i)
	{
		int baseLine = _axis->fontMetrics().height()/2  - _axis->fontMetrics().descent();
		painter.drawLine(-QSelfAdjustingAxis::scaleLen, -_axis->deltaY()*i, 0, -_axis->deltaY()*i);
		painter.drawText(-_axis->fontMetrics().width(_axis->values().at(i))-QSelfAdjustingAxis::scaleLen, -_axis->deltaY()*i+baseLine, _axis->values().at(i));
	}
}

void QChart::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	if (event->orientation() == Qt::Vertical)
	{
		_axis->adjust(numSteps);
		update();
	}

	event->accept();
}