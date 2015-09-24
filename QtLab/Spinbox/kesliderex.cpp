#include "kesliderex.h"

void CKESliderEx::Init(double dStart, double dEnd,double dStep,int nCount)
{
	setRange(0, nCount - 1);
	m_dStep  = dStart <= dEnd ? dStep : -dStep;
	m_dStart = dStart;
	m_dEnd   = dEnd;
}

double CKESliderEx::GetCurValue()
{
	return m_dStart + value() * m_dStep;
}

void CKESliderEx::SetValue(double dValue)
{
	setValue(qRound((qBound(m_dStart, dValue, m_dEnd) - m_dStart) / m_dStep));
}