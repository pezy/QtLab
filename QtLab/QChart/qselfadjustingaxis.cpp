#include "qselfadjustingaxis.h"
#include <QPoint>
#include <QFontMetrics>
#include <QDebug>

#include "qchar.h"

QSelfAdjustingAxis::QSelfAdjustingAxis(QBarSeries *series, const QStringList &categories, const QFont &font)
	: _font(font), _categories(categories), _hasRect(false), _defaultFactor(4)
{
	_fontMetrics = new QFontMetrics(font);
	_min = _max = 0;
	_factor = _defaultFactor;
	
	/*!
	 * Calculate the min/max value of whole data.
	 */

	for (int i=0; i != series->size(); ++i)
	{
		_max = qMax(_max, *std::max_element(series->at(i)->begin(), series->at(i)->end()));
		_min = qMin(_min, *std::min_element(series->at(i)->begin(), series->at(i)->end()));
	}

	_actualMax = _max;
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
	_basePoint.setX(rect.left() + xEdge + yTextWidth + scaleLen);
	_width = rect.width() - 2*xEdge - yTextWidth - scaleLen - 1;

	const int yEdge = 10;
	int xTextHeight = _fontMetrics->height();
	_basePoint.setY(rect.bottom() - yEdge - xTextHeight - scaleLen);
	_height = rect.height() - 2*yEdge - xTextHeight - scaleLen - 2;

	/*!
	 * Calculate the delta of each coordinate.
	 */
	_deltaX = static_cast<float>(_width / _categories.size());
	_deltaYMin = xTextHeight;

	autoCalculateYTick();

	_hasRect = true;
}

float QSelfAdjustingAxis::valueToTick(int value) const
{
	float ratio = static_cast<float>(value)/_max;
	return _height*ratio;
}

void QSelfAdjustingAxis::adjust(int numStep)
{
	_factor += numStep;
	_max += numStep;
	if (_max < _actualMax) _max = _actualMax;	/**< check the max value. */
	autoCalculateYTick();
}

void QSelfAdjustingAxis::autoCalculateYTick()
{
	if (_factor < _defaultFactor) _factor = _defaultFactor;	/**< avoid sparse */

	_deltaY = static_cast<float>(_height/_factor);
	if (_deltaY < _deltaYMin) /**< avoid dense */
	{
		_factor = _defaultFactor;
		_deltaY = static_cast<float>(_height/_factor);
	}
	
	/*!
	 * Set scale mark on the Y-axis.
	 */
	_values.clear();
	float valueDelta = static_cast<float>(_max - _min);
	for (int i = 0; i != _factor; ++i)
		_values << QString::number(_min + i*valueDelta/_factor, 'f', 1);
	_values << QString::number(static_cast<float>(_max), 'f', 1);
}