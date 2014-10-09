#ifndef QDATA_H_
#define QDATA_H_

#include <QList>
#include <QColor>
#include <QVector>

/*!
 * \class QBarSet
 *
 * \brief The value set of the chart.
 *
 * \author pezy
 * \date Oct. 2014
 */

class QBarSet : public QList<int>
{
public:
	QBarSet(const QString &name, const QColor &color):_name(name),_color(color) {}
	~QBarSet(){}
	QString name() const { return _name; }
	QColor color() const { return _color; }

private:
	QString _name;
	QColor _color;
};

typedef QVector<QBarSet*> QBarSeries;

#endif