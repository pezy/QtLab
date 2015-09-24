#ifndef KESLIDEREX_H
#define KESLIDEREX_H

#include <QSlider>

/**
* @class CKESliderEx kesliderex.h
* @brief The class define new slider.
* 
* @author daily 
* @date 7/6/2012
* @update pezy
* @date 2015-09-17
* @see 
*/

class CKESliderEx : public QSlider
{
public:
	explicit CKESliderEx(QWidget *parent = nullptr) : QSlider(parent) {}
	~CKESliderEx(void){}
	void Init( double dStart, double dEnd,double fStep,int nCount);
	double GetCurValue();
	void SetValue(double dValue);
private:
	double m_dStep  = 1.0;
	double m_dStart = 0.0;
	double m_dEnd   = 1.0;
};

#endif // KESLIDEREX_H
