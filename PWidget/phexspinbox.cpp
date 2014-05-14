#include "phexspinbox.h"

PHexSpinBox::PHexSpinBox(QWidget *parent)
	: QSpinBox(parent)
{
	setRange(0, 255);
	validator = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{1,8}"), this);
}

QValidator::State PHexSpinBox::validate(QString &text, int &pos) const
{
	return validator->validate(text, pos);
}

QString PHexSpinBox::textFromValue(int value) const
{
	return QString::number(value, 16).toUpper();
}

int PHexSpinBox::valueFromText(const QString &text) const
{
	bool ok;
	return text.toInt(&ok, 16);
}


