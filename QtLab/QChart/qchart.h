#ifndef QCHART_H
#define QCHART_H

#include <QWidget>
#include "qdata.h"

/*!
 * \class QChart
 *
 * \brief Simple custom chart widget.
 *
 * \author PEZY
 * \date Sept. 2014
 */
class QSelfAdjustingAxis;

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
	void setAxis(QSelfAdjustingAxis *axis) { _axis = axis; }

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

private:
	const int titleHeight;
	const int tipHeight;

	QString _title;
	QSelfAdjustingAxis *_axis;
	QBarSeries *_series;
};

#endif // QCHART_H
