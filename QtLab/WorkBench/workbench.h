#ifndef WORKBENCH_H
#define WORKBENCH_H

#include <QtWidgets/QWidget>
#include <QButtonGroup>

class WorkBench : public QWidget
{
	Q_OBJECT

public:
	WorkBench(QWidget *parent = 0);
	~WorkBench();

protected:
	void keyPressEvent(QKeyEvent* e);

private:
	void _Init();
	void _CheckLicense();

private slots:
	void slotProcess();

private:
	QButtonGroup m_group;
};

#endif // WORKBENCH_H
