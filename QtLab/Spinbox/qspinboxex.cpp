#include "qspinboxex.h"

QSpinBoxEx::QSpinBoxEx(QWidget *parent)
	: QDoubleSpinBox(parent)
{

}

QSpinBoxEx::~QSpinBoxEx()
{

}

void QSpinBoxEx::Init(double dStart, double dEnd, double dStep, int nCount)
{
	setRange(0, nCount - 1);
	m_dStep = dStart <= dEnd ? dStep : -dStep;
	m_dStart = dStart;
	m_dEnd = dEnd;
}

double QSpinBoxEx::GetCurValue()
{
	return m_dStart + value() * m_dStep;
}

void QSpinBoxEx::SetValue(double dValue)
{
	setValue((qBound(qMin(m_dStart, m_dEnd), dValue, qMax(m_dStart, m_dEnd)) - m_dStart) / m_dStep);
}

QString QSpinBoxEx::textFromValue(double dValue) const
{
	return QString::number(m_dStart + dValue * m_dStep);
}