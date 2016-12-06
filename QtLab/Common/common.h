#ifndef COMMON_H
#define COMMON_H

#include "common_global.h"
#include <QList>
#include <QColor>

namespace NCommon
{
	QList<QColor> COMMON_EXPORT GenerateDistinctColorPalette(int nColor);
	QList<QColor> COMMON_EXPORT GetkellyColours();

};

#endif // COMMON_H
