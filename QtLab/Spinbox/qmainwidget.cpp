#include "qmainwidget.h"

QMainWidget::QMainWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.m_slider->Init(-1.0, -10.0, 1.0, 10);
	ui.m_spin->Init(-1.0, -10.0, 1.0, 10);
	connect(ui.m_slider, SIGNAL(valueChanged(int)), this, SLOT(slotSetValue(int)));
	connect(ui.m_reset, SIGNAL(clicked()), this, SLOT(slotReset()));
}

QMainWidget::~QMainWidget()
{

}

void QMainWidget::slotSetValue(int value)
{
	ui.m_text->setText(QString::number(ui.m_slider->GetCurValue()));
}

void QMainWidget::slotReset()
{
	ui.m_spin->SetValue(-5.0);
}