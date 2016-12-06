#include "randomcolor.h"
#include "common.h"

RandomColor::RandomColor(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);
	_Init();
}

RandomColor::~RandomColor()
{

}

void RandomColor::_Init()
{
	connect(m_ui.m_pActionTest, &QAction::triggered, this, &RandomColor::slotRandom);
	connect(m_ui.m_pActionKellyColors, &QAction::triggered, this, &RandomColor::slotKellyColors);
}

void RandomColor::_SetLabelColor(QLabel * pLabel, const QColor & color)
{
	QPalette palette = pLabel->palette();
	palette.setColor(pLabel->backgroundRole(), color);
	pLabel->setAutoFillBackground(true);
	pLabel->setPalette(palette);
}

void RandomColor::slotKellyColors()
{
	const QList<QColor>& listColor = NCommon::GetkellyColours();
	const QList<QLabel*>& listLabel = m_ui.m_pCentralWidget->findChildren<QLabel*>();

	for (int i = 0; i != 20; ++i)
	{
		_SetLabelColor(listLabel.at(i), listColor.at(i));
		listLabel.at(i)->setText(listColor.at(i).name());
	}
}

void RandomColor::slotRandom()
{
	const QList<QColor>& listColor = NCommon::GenerateDistinctColorPalette(20);
	const QList<QLabel*>& listLabel = m_ui.m_pCentralWidget->findChildren<QLabel*>();

	for (int i = 0; i != 20; ++i)
	{
		_SetLabelColor(listLabel.at(i), listColor.at(i));
		listLabel.at(i)->setText(listColor.at(i).name());
	}
}
