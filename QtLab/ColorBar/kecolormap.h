#ifndef CKECOLORMAP_H
#define CKECOLORMAP_H

#include <qstringlist.h>
#include <qcolor.h>

/*!
* @class	CKEColormap provided in the file "kecolormap.h"
* @brief	Used to represent a 256 cells colormap.
* @author	pezy
* @date		2016-01-26
* @see		
*/

class CKEColormap
{
public:
	static bool InitializeColormapsFromFile();
    static bool InitializeColormapsFromDB();
    
	static QStringList GetAllColormapsName();
    
	static CKEColormap* GetColormap(const QString& sName);
	static CKEColormap* GetColormap(unsigned int iIDX);

public:
	QColor GetColorAt(unsigned char iIDX);
    void SetColorAt(unsigned char iIDX, const QColor& color);

	void SaveColormap(QString name, QList<QColor> &vecColor);

	QString GetName()
	{
		return m_strName;
	}

private:
    //! @obsolete 2015-01-26. use db to save colormap.
	CKEColormap(const QString& sFile);
    //! @constructor
    CKEColormap(const QString& sName, const QColor (&arrColor)[256]);
    CKEColormap(const QString& sName, const QRgb(&arrRgbs)[256]);

	QString		m_strName;
	unsigned char	m_iRed[256];
	unsigned char	m_iGreen[256];
	unsigned char	m_iBlue[256];
	unsigned char   m_iAlpha[256];

	static QList<CKEColormap>	m_staListColormap;
};

#endif // CKECOLORMAP_H