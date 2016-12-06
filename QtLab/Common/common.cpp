#include "common.h"

QList<QColor> COMMON_EXPORT NCommon::GenerateDistinctColorPalette(int nColor)
{
	QStringList listColorName = QColor::colorNames();

	listColorName.removeOne("aliceblue");
	listColorName.removeOne("azure");
	listColorName.removeOne("black");
	listColorName.removeOne("floralwhite");
	listColorName.removeOne("ghostwhite");
	listColorName.removeOne("honeydew");
	listColorName.removeOne("lavenderblush");
	listColorName.removeOne("mintcream");
	listColorName.removeOne("snow");
	listColorName.removeOne("white");
	listColorName.removeOne("whitesmoke");

	QList<QColor> listColor;

	for (int i = 0; i != nColor; )
	{
		int indexSeed = qRound((double)qrand() / RAND_MAX * (listColorName.size() - 1));
		QString colorName = listColorName.at(indexSeed);
		if (listColor.contains(colorName))
			continue;

		listColor << QColor(colorName);
		++i;
	}
	
	return listColor;
}

QList<QColor>COMMON_EXPORT NCommon::GetkellyColours()
{
	// check http://stackoverflow.com/questions/470690/how-to-automatically-generate-n-distinct-colors

	return 
	{
		QColor(0xFFB300),
		QColor(0x803E75),
		QColor(0xFF6800),
		QColor(0xA6BDD7),
		QColor(0xC10020),
		QColor(0xCEA262),
		QColor(0x817066),
		QColor(0x007D34),
		QColor(0xF6768E),
		QColor(0x00538A),
		QColor(0xFF7A5C),
		QColor(0x53377A),
		QColor(0xFF8E00),
		QColor(0xB32851),
		QColor(0xF4C800),
		QColor(0x7F180D),
		QColor(0x93AA00),
		QColor(0x593315),
		QColor(0xF13A13),
		QColor(0x232C16)
	};
}
