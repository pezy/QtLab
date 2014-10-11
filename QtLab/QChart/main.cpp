#include <QApplication>
#include <QMainWindow>
#include "qchart.h"
#include "qselfadjustingaxis.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QBarSet *set0 = new QBarSet("Jane", Qt::darkCyan);
	QBarSet *set1 = new QBarSet("John", Qt::darkGreen);
	QBarSet *set2 = new QBarSet("Axel", Qt::darkYellow);
	QBarSet *set3 = new QBarSet("Mary", Qt::darkMagenta);
	QBarSet *set4 = new QBarSet("Samantha", Qt::darkRed);

	*set0 << 1 << 2 << 3 << 4 << 5 << 6;
	*set1 << 5 << 0 << 0 << 4 << 0 << 7;
	*set2 << 3 << 5 << 8 << 13 << 8 << 5;
	*set3 << 5 << 6 << 7 << 3 << 4 << 5;
	*set4 << 9 << 7 << 5 << 3 << 1 << 2;

	QBarSeries *series = new QBarSeries();
	series->append(set0);
	series->append(set1);
	series->append(set2);
	series->append(set3);
	series->append(set4);

	QChart *chart = new QChart;
	chart->addSeries(series);
	chart->setTitle("Simple barchart example");

	QStringList categories;
	categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
	QSelfAdjustingAxis *axis = new QSelfAdjustingAxis(series, categories, QFont("Microsoft YaHei UI", 9));
	chart->setAxis(axis);

	QMainWindow window;
	window.setCentralWidget(chart);
	window.resize(420, 300);
	window.show();

    auto ret = a.exec();

    delete set0;
    delete set1;
    delete set2;
    delete set3;
    delete set4;

    delete series;
    delete chart;
    delete axis;

    return ret;
}
