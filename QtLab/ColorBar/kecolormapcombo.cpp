#include "kecolormapcombo.h"
#include "kecolormap.h"
#include <cassert>
#include <QPainter>

CKEColormapCombo::CKEColormapCombo(QWidget *parent)
	: CKEBaseComboBox(parent), m_pIconSize(55, 10)
{
	setIconSize(m_pIconSize);

	QStringList strColormapName = CKEColormap::GetAllColormapsName();
	for (int i=0; i<strColormapName.size(); ++i)
		addItem(_Icon(strColormapName.at(i)), strColormapName.at(i));
	this->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	this->setMinimumWidth(170);
}

QIcon CKEColormapCombo::_Icon(const QString& name)
{
	return QIcon(_Pixmap(name));
}

QPixmap CKEColormapCombo::_Pixmap(const QString& name)
{
	QPixmap pixmap(m_pIconSize);
	QPainter painter(&pixmap);

    CKEColormap *colormap = CKEColormap::GetColormap(name);
	QLinearGradient gradient(0, m_pIconSize.height()/2, m_pIconSize.width(), m_pIconSize.height()/2);

	for (unsigned i=0; i<256; ++i)
	{
		gradient.setColorAt(i/255.0f, colormap->GetColorAt(static_cast<unsigned char>(i)));
	}

	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(QBrush(gradient), m_pIconSize.height())); 
	painter.drawLine(0, m_pIconSize.height()/2, m_pIconSize.width(), m_pIconSize.height()/2);

	return pixmap;
}

QColor CKEColormapCombo::GetColor( int nIndex )
{
	QColor color;

    CKEColormap *pColormap = CKEColormap::GetColormap(m_Text);
	assert( pColormap );
	if( pColormap )
	{
		//std::vector<int>vColor;
		color=pColormap->GetColorAt( nIndex );

		/*assert( nIndex < vColor.size() );
		if( nIndex < vColor.size() )
		{
			color.setRgb( vColor[nIndex] );
		}*/
	}

	return color;
}