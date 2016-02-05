#ifndef KECOLORMAPCOMBO_H
#define KECOLORMAPCOMBO_H

#include <QComboBox>
#include "kebasecombobox.h"

class CKEColormapCombo : public CKEBaseComboBox
{
public:
	CKEColormapCombo(QWidget *parent = 0);
	virtual ~CKEColormapCombo(){}
	void SetCurrentColormapText(const QString& text){SetCurrentText(text);m_Text=text;}
	QColor GetColor( int nIndex );
private:
	QIcon _Icon(const QString& name); 
	QPixmap _Pixmap(const QString& name);

private:
	QString m_Text;
	QSize m_pIconSize;
};

#endif // RLCOLORMAPCOMBO_H
