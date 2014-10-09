#include "qselfadjustingaxis.h"
#include <QPoint>
#include <QFontMetrics>
#include <QDebug>

#include "qchar.h"

QSelfAdjustingAxis::QSelfAdjustingAxis(QBarSeries *series, const QStringList &categories, const QFont &font)
	: _categories(categories), _hasRect(false)
{
	_fontMetrics = new QFontMetrics(font);
	_min = _max = 0;
	
	/*!
	 * Calculate the min/max value of whole data.
	 */

	for (int i=0; i != series->size(); ++i)
	{
		_max = qMax(_max, *std::max_element(series->at(i)->begin(), series->at(i)->end()));
		_min = qMin(_min, *std::min_element(series->at(i)->begin(), series->at(i)->end()));
	}
}

QSelfAdjustingAxis::~QSelfAdjustingAxis()
{
	delete _fontMetrics;
}

void QSelfAdjustingAxis::setRect(const QRect &rect)
{
	/*!
	 * Calculate the base point and width/height
	 */
	const int xEdge = 35;
	int yTextWidth = _fontMetrics->width(QString::number(_max) + ".0");
	_basePoint.setX(xEdge + yTextWidth + scaleLen);
	_width = rect.width() - 2*xEdge - yTextWidth - scaleLen;

	const int yEdge = 10;
	int xTextHeight = _fontMetrics->height();
	_basePoint.setY(rect.bottom() - yEdge);
	_height = rect.height() - 2*yEdge - xTextHeight - scaleLen;

	/*!
	 * Calculate the delta of each coordinate.
	 */
	_deltaX = static_cast<float>(_width / _categories.size());
	_deltaY = static_cast<float>(_height/ 4);
	_deltaYMin = xTextHeight*2;

	/*!
	 * Set scale mark on the Y-axis.
	 */
	float valueDelta = static_cast<float>(_max - _min);
	for (int i = 0; i != 4; ++i)
		_values << QString::number(_min + i*valueDelta/4, 'f', 1);
	_values << QString::number(static_cast<float>(_max), 'f', 1);

	_hasRect = true;
}

float QSelfAdjustingAxis::valueToTick(int value) const
{
	float ratio = static_cast<float>(value)/_max;
	return _height*ratio;
}