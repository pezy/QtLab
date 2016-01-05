#ifndef _CKECOLORMAP_H__
#define _CKECOLORMAP_H__

#include <qstringlist.h>
#include <qcolor.h>

///@brief	Used to represent a 256 cells colormap.
///@author	Xiong Xiong
///@date	Sep, 2013
///@see		
class CKEColormap
{
public:
	static bool InitializeColormaps();

	static QStringList GetAllColormapsName();

	static CKEColormap* GetColormap(const QString& sName);

	static CKEColormap* GetColormap(unsigned int iIDX);

	QColor GetColorAt(unsigned char iIDX);

	void SaveColormap(QString name, QList<QColor> &vecColor);

	QString GetName()
	{
		return m_strName;
	}

private:
	CKEColormap(const QString& sFile);
    CKEColormap(const QString& sName, const QColor (&arrColor)[256]);

	QString		m_strName;
	unsigned char	m_iRed[256];
	unsigned char	m_iGreen[256];
	unsigned char	m_iBlue[256];
	unsigned char   m_iAlpha[256];

	static QList<CKEColormap>	m_staListColormap;
};

#endif