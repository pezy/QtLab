#include "kecolormap.h"
#include <qdir.h>
#include <qrgb.h>
#include <vector>
#include <QtSql>
#include <QDebug>

#include "kelog.h"
#include "kedbio.h"
#include "kesaveload.h"

QList<CKEColormap> CKEColormap::m_staListColormap;

bool CKEColormap::InitializeColormapsFromDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(CKEDBIO::GetDefaultDBFileName());

    if (!db.open())
    {
        CKELog::AddLog("Open DB error: " + CKEDBIO::GetDefaultDBFileName());
        qDebug() << db.lastError();
        return false;
    }

    QRgb rgbs[256];

    QSqlQueryModel model;
    model.setQuery("Select * from Colormap");
    for (int i = 0; i < model.rowCount(); ++i)
    {
        QString colormapName = model.record(i).value("ID").toString();
        LoadDataFromBLOB(model.record(i).value("ColormapData"), rgbs, 256);
        CKEColormap colormap(colormapName, rgbs);
        m_staListColormap << colormap;
    }

    return true;
}

bool CKEColormap::InitializeColormapsFromFile()
{
    QString strColormapDir = QDir::currentPath() + QDir::separator() + "Colormap" + QDir::separator();
	QDir qDir(strColormapDir);
	if(!qDir.exists())
	{
        CKELog::AddLog("Can not find the colormap directory at " + strColormapDir + "!");
		return false;
	}

	QStringList sListFilter;
	sListFilter<<"*.pal";
	QStringList sListFiles = qDir.entryList(sListFilter, QDir::Files);
	int i, iNum = sListFiles.size();

	if(iNum == 0)
	{
		CKELog::AddLog("No colormap files found at " + strColormapDir + "!");
		return false;
	}

	for(i = 0; i < iNum; i++)
	{
		CKEColormap colormap(strColormapDir + sListFiles.at(i));
		if(colormap.GetName() != "")
			m_staListColormap<<colormap;
	}

	return true;
}

QStringList CKEColormap::GetAllColormapsName()
{
	QStringList strListName;

	for(int i = 0; i< m_staListColormap.size(); i++)
	{
		strListName<<m_staListColormap[i].GetName();
	}

	return strListName;
}

CKEColormap::CKEColormap(const QString& strFile)
{
	// set name
	QFileInfo qFI(strFile);
	m_strName = qFI.baseName();

	// open file
	QFile qFile(strFile);
	if (!qFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		Q_ASSERT(false);
	}
	
	// read head
	QString strLine;		
	while(!qFile.atEnd())
	{
		strLine = qFile.readLine();
		if (strLine.contains("<ColorMap>"))
			break;
	}

	// read data
	QStringList strListLine;
	std::vector<unsigned char> vecA, vecR, vecG, vecB;
	while(!qFile.atEnd())
	{
		strLine = qFile.readLine();
		if (strLine.contains("</ColorMap>"))
			break;

		strLine.remove("Color", Qt::CaseInsensitive);
		strLine.remove("=", Qt::CaseInsensitive);
		strLine = strLine.simplified();
		strListLine = strLine.split(',', QString::SkipEmptyParts);
		if (strListLine.size() < 4)
			break;

		vecA.push_back(strListLine[0].toShort());
		vecR.push_back(strListLine[1].toShort());
		vecG.push_back(strListLine[2].toShort());
		vecB.push_back(strListLine[3].toShort());
	}

	int nSize = (int) vecA.size();
	int IInterval = 256/nSize;
	for (int i=0; i<nSize; ++i)
	{
		m_iAlpha[i*IInterval]	= vecA[i];
		m_iRed[i*IInterval]		= vecR[i];
		m_iGreen[i*IInterval]	= vecG[i];
		m_iBlue[i*IInterval]	= vecB[i];

		if (i < nSize -1)
		{
			for (int j=i*IInterval+1; j<(i+1)*IInterval; ++j)
			{
				float fFactor = (j - i*IInterval)*1.f/IInterval;
				m_iAlpha[j]	= vecA[i]*(1.f - fFactor) + vecA[i+1]*fFactor;
				m_iRed[j]	= vecR[i]*(1.f - fFactor) + vecR[i+1]*fFactor;
				m_iGreen[j]	= vecG[i]*(1.f - fFactor) + vecG[i+1]*fFactor;
				m_iBlue[j]	= vecB[i]*(1.f - fFactor) + vecB[i+1]*fFactor;
			}

		}
	}

	m_iAlpha[255]	= m_iAlpha[254];
	m_iRed[255]		= m_iRed[254];
	m_iGreen[255]	= m_iGreen[254];
	m_iBlue[255]	= m_iBlue[254];

	qFile.close();
}

CKEColormap::CKEColormap(const QString& sName, const QColor(&arrColor)[256]) : m_strName(sName)
{
    for (int i = 0; i < 256; ++i)
    {
        m_iRed[i] = qRed(arrColor[i].rgba());
        m_iGreen[i] = qGreen(arrColor[i].rgba());
        m_iBlue[i] = qBlue(arrColor[i].rgba());
        m_iAlpha[i] = qAlpha(arrColor[i].rgba());
    }
}

CKEColormap::CKEColormap(const QString& sName, const QRgb(&arrRgbs)[256]) : m_strName(sName)
{
    for (int i = 0; i < 256; ++i)
    {
        m_iRed[i] = qRed(arrRgbs[i]);
        m_iGreen[i] = qGreen(arrRgbs[i]);
        m_iBlue[i] = qBlue(arrRgbs[i]);
        m_iAlpha[i] = qAlpha(arrRgbs[i]);
    }
}

CKEColormap* CKEColormap::GetColormap(const QString& strName)
{
	for(int i = 0; i< m_staListColormap.size(); i++)
	{
		if(m_staListColormap[i].GetName() == strName)
			return &(m_staListColormap[i]);
	}

	return NULL;
}

CKEColormap* CKEColormap::GetColormap(unsigned int iIDX)
{
	if(iIDX >= m_staListColormap.size())
		return NULL;
	else
		return &(m_staListColormap[iIDX]);
}

QColor CKEColormap::GetColorAt(unsigned char iIDX)
{
	return QColor(m_iRed[iIDX], m_iGreen[iIDX], m_iBlue[iIDX], m_iAlpha[iIDX]);
}

void CKEColormap::SetColorAt(unsigned char iIDX, const QColor& color)
{
    m_iRed[iIDX] = qRed(color.rgba());
    m_iGreen[iIDX] = qGreen(color.rgba());
    m_iBlue[iIDX] = qBlue(color.rgba());
    m_iAlpha[iIDX] = qAlpha(color.rgba());
}

void CKEColormap::SaveColormap(QString name, QList<QColor> &vecColor)
{
    Q_ASSERT_X(vecColor.size() == 256, "colormap", "color's quantity illegal(should be 256).");


}