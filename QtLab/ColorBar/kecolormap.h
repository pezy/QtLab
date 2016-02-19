#ifndef CKECOLORMAP_H
#define CKECOLORMAP_H

#include <qstringlist.h>
#include <qcolor.h>
#include <qmap.h>
#include <qjsonobject.h>
#include "rapidjson\document.h"

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
	//static bool InitializeColormapsFromFile();
    //static bool InitializeColormapsFromDB();
    static bool InitializeColormapsFromJason();
    
	static QStringList GetAllColormapsName();
    
	static CKEColormap* GetColormap(const QString& sName);
	static CKEColormap* GetColormap(unsigned int iIDX);

public:
    // copy
    CKEColormap(const CKEColormap& colormap);
    CKEColormap& operator=(const CKEColormap& colormap);

	QColor GetColorAt(int index) const;
    QString GetName() const;
    QList<int> GetControlPointsIndex() const;
    int GetColorNum() const;

    bool SaveAs();

    void SetControlPointRgb(uchar index, const QRgb& rgb);
    void Flip();

private:
	//CKEColormap(const QString& sFile);
    CKEColormap(const QString& sName, const QMap<int, QRgb>& mapRgb, const QMap<int, uchar>& mapAlpha, int colNum = 256, QColor invalidCol = QColor(Qt::white));

    void _UpdateColormap();
    void _WriteToJson(QJsonObject &colormap);
    void _WriteToJson(rapidjson::Document &colormap);

    QString m_strName;
    int m_colorNum;
    QColor m_invalidColor;

    QMap<int, QRgb> m_mapControlPointsRgb;
    QMap<int, uchar> m_mapControlPointsAlpah;

    QColor m_listColor[256];

	static QList<CKEColormap> m_stdListColormap;
};

#endif // CKECOLORMAP_H