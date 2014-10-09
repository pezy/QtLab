#ifndef QCHART_H
#define QCHART_H

#include <QWidget>
#include <QSet>

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

/*!
 * \class QChart
 *
 * \brief Simple custom chart widget.
 *
 * \author PEZY
 * \date Sept. 2014
 */

class QChart : public QWidget
{
    Q_OBJECT

	Q_PROPERTY(QString title READ title WRITE setTitle);
    
public:
    QChart(QWidget *parent = 0);
    ~QChart();

	QString title() const { return _title; }
	void setTitle(const QString &title) { _title = title; }

	void addSeries(QBarSeries *series) { _series = series; }

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

private:
	QString _title;
	QBarSeries *_series;
};

#endif // QCHART_H
