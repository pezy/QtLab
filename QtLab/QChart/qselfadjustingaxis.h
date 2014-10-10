#ifndef QSELFADJUSTINGAXIS_H
#define QSELFADJUSTINGAXIS_H

#include <QRect>
#include <QFont>
#include <QPoint>
#include "qdata.h"

/*!
 * \class QSelfAdjustingAxis
 *
 * \brief Draw the axis which is self-adjusting.
 *
 * \author pezy
 * \date Oct. 2014
 */

class QFontMetrics;

class QSelfAdjustingAxis
{
public:
	QSelfAdjustingAxis(QBarSeries *series, const QStringList &categories, const QFont &font);
	~QSelfAdjustingAxis();

	void setRect(const QRect &rect);

	int width() const { return _width; }
	int height() const { return _height; }
	float deltaX() const { return _deltaX; }
	float deltaY() const { return _deltaY; }
	const QPoint &basePoint() { return _basePoint; }
	const QStringList &categories() { return _categories; }
	const QStringList &values() { return _values; }
	const QFont &font() { return _font; }
	const QFontMetrics &fontMetrics() { return *_fontMetrics; }
	bool hasRect() const { return _hasRect;}
	float valueToTick(int value) const;
	void adjust(int numStep);

private:
	void autoCalculateYTick();

public:
	static const int scaleLen = 4;

private:
	QFont _font;
	QFontMetrics *_fontMetrics;
	int _width;
	int _height;
	int _min;
	int _max;
	int _actualMax;
	float _deltaX;
	float _deltaY;
	int _deltaYMin;

	QPoint _basePoint;
	QStringList _categories;
	QStringList _values;
	bool _hasRect;

	const int _defaultFactor;
	int _factor;
};

#endif // QSELFADJUSTINGAXIS_H
