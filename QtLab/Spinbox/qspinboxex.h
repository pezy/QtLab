#ifndef QSPINBOXEX_H
#define QSPINBOXEX_H

#include <QDoubleSpinBox>

class QSpinBoxEx : public QDoubleSpinBox
{
	Q_OBJECT

public:
	QSpinBoxEx(QWidget *parent);
	~QSpinBoxEx();

	void Init(double dStart, double dEnd, double dStep, int nCount);
	double GetCurValue();
	void SetValue(double dValue);

	virtual QString	textFromValue(double value) const override;

private:
	double m_dStep = 1.0;
	double m_dStart = 0.0;
	double m_dEnd = 1.0;
};

#endif // QSPINBOXEX_H
