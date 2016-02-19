#include "kecolormap.h"
#include <qdir.h>
#include <qrgb.h>
#include <vector>
#include <QtSql>
#include <QVector3D>

#include "rapidjson\prettywriter.h"

#include "kelog.h"
#include "kedbio.h"
#include "kesaveload.h"

QList<CKEColormap> CKEColormap::m_stdListColormap;

bool CKEColormap::InitializeColormapsFromJason()
{
    const QString& jasonColorScalesDir = "C:\\Program Files\\JasonSW\\Jason 8.4\\etc\\colorscales";

    QDir qDir(jasonColorScalesDir);
    if (!qDir.exists())
    {
        CKELog::AddLog("Can not find the jason 8.4 colormap directory at " + jasonColorScalesDir + "!");
        return false;
    }

    QStringList sListFilter;
    sListFilter << "*.shs";
    QStringList sListFiles = qDir.entryList(sListFilter, QDir::Files);
    
    if (sListFiles.isEmpty())
    {
        CKELog::AddLog("No colormap files found at " + jasonColorScalesDir + "!");
        return false;
    }

    for (const auto& fileName : sListFiles)
    {
        QString fileDir = jasonColorScalesDir + QDir::separator() + fileName;

        QFileInfo fileInfo(fileDir);

        // open file
        QFile file(fileDir);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            Q_ASSERT(false);
        }

        // read control points
        bool bRgb = false;
        bool bAlpha = false;
        QMap<int, QRgb> mapRgb;
        QMap<int, uchar> mapAlpha;

        int numCol = 0;
        qreal pos, r, g, b, a;

        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString line = stream.readLine();

            if (line.section(':', 0, 0) == "number_of_colors")
            {
                numCol = line.section('=', 1).remove(';').trimmed().toInt();
            }

            if (line.section(':', 0, 0) == "rgb_control_points") // rgb begin
            {
                bRgb = true;
            }
            else if (line.section(':', 0, 0) == "alpha_control_points") // alpha begin
            {
                bAlpha = true;
            }
            else if (line == ";")
            {
                bRgb = false;
                bAlpha = false;
            }

            if (bRgb)
            {
                if (line.contains("fraction:real"))
                {
                    pos = line.section('=', 1).remove(';').trimmed().toDouble();
                }
                else if (line.contains("red:real"))
                {
                    r = line.section('=', 1).remove(';').trimmed().toDouble();
                }
                else if (line.contains("green:real"))
                {
                    g = line.section('=', 1).remove(';').trimmed().toDouble();
                }
                else if (line.contains("blue:real"))
                {
                    b = line.section('=', 1).remove(';').trimmed().toDouble();

                    QColor color(qRound(r / 100 * 255), qRound(g / 100 * 255), qRound(b / 100 * 255));
                    mapRgb.insert(qRound(pos * (numCol - 1)), color.rgb());
                }
            }
            
            if (bAlpha)
            {
                if (line.contains("fraction:real"))
                {
                    pos = line.section('=', 1).remove(';').trimmed().toDouble();
                }
                else if (line.contains("alpha:real"))
                {
                    a = line.section('=', 1).remove(';').trimmed().toDouble();

                    mapAlpha.insert(qRound(pos * (numCol - 1)), qBound((uchar)0, (uchar)(a / 100 * 255), (uchar)255));
                }
            }
        }
       
        CKEColormap colormap(fileInfo.baseName(), mapRgb, mapAlpha, numCol);
        m_stdListColormap << colormap;
    }

    return true;
}

QStringList CKEColormap::GetAllColormapsName()
{
	QStringList strListName;

	for(int i = 0; i< m_stdListColormap.size(); i++)
	{
		strListName<<m_stdListColormap[i].GetName();
	}

	return strListName;
}

CKEColormap* CKEColormap::GetColormap(const QString& strName)
{
	for(int i = 0; i< m_stdListColormap.size(); i++)
	{
		if(m_stdListColormap[i].GetName() == strName)
			return &(m_stdListColormap[i]);
	}

	return NULL;
}

CKEColormap* CKEColormap::GetColormap(unsigned int iIDX)
{
	if(iIDX >= m_stdListColormap.size())
		return NULL;
	else
		return &(m_stdListColormap[iIDX]);
}

QColor CKEColormap::GetColorAt(int index) const
{
    Q_ASSERT(0 <= index && index < m_colorNum);

    return m_listColor[index];
}

QString CKEColormap::GetName() const
{
    return m_strName;
}

QList<int> CKEColormap::GetControlPointsIndex() const
{
    return m_mapControlPointsRgb.keys();
}

int CKEColormap::GetColorNum() const
{
    return m_colorNum;
}

bool CKEColormap::SaveAs()
{
    QString strFilePath = QDir::currentPath() + QDir::separator() + "Colormap" + QDir::separator() + m_strName + ".colormap.json";

    QFile saveFile(strFilePath);
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return false;
    }

    rapidjson::Document colormap;
    _WriteToJson(colormap);

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    colormap.Accept(writer);
    saveFile.write(sb.GetString());

    return true;
}

void CKEColormap::SetControlPointRgb(uchar index, const QRgb& rgb)
{
    m_mapControlPointsRgb.insert(index, rgb);

    _UpdateColormap();
}

void CKEColormap::Flip()
{
    QList<int> indices = m_mapControlPointsRgb.keys();
    QList<QRgb> rgbs = m_mapControlPointsRgb.values();

    m_mapControlPointsRgb.clear();

    for (auto i = 0; i != indices.size(); ++i)
    {
        m_mapControlPointsRgb.insert(m_colorNum - indices.at(i) - 1, rgbs.at(i));
    }

    indices = m_mapControlPointsAlpah.keys();
    QList<uchar> alphas = m_mapControlPointsAlpah.values();

    m_mapControlPointsAlpah.clear();

    for (auto i = 0; i != indices.size(); ++i)
    {
        m_mapControlPointsAlpah.insert(m_colorNum - indices.at(i) - 1, alphas.at(i));
    }

    _UpdateColormap();
}

CKEColormap::CKEColormap(const QString& sName, const QMap<int, QRgb>& mapRgb, const QMap<int, uchar>& mapAlpha, int colNum, QColor invalidCol)
    :m_strName(sName)
{
    if (mapRgb.size() < 2)
        throw "At least have two control points.";

    m_mapControlPointsRgb = mapRgb;
    m_mapControlPointsAlpah = mapAlpha;
    m_colorNum = colNum;
    m_invalidColor = invalidCol;

    _UpdateColormap();
}

CKEColormap::CKEColormap(const CKEColormap& colormap)
    : m_strName(colormap.m_strName)
    , m_mapControlPointsRgb(colormap.m_mapControlPointsRgb)
    , m_mapControlPointsAlpah(colormap.m_mapControlPointsAlpah)
    , m_colorNum(colormap.m_colorNum)
    , m_invalidColor(colormap.m_invalidColor)
{
    for (auto i = 0; i != m_colorNum; ++i)
    {
        m_listColor[i] = colormap.GetColorAt(i);
    }
}

CKEColormap& CKEColormap::operator = (const CKEColormap& colormap)
{
    m_strName = colormap.m_strName;
    m_mapControlPointsRgb = colormap.m_mapControlPointsRgb;
    m_mapControlPointsAlpah = colormap.m_mapControlPointsAlpah;
    m_colorNum = colormap.m_colorNum;
    m_invalidColor = colormap.m_invalidColor;

    for (auto i = 0; i != m_colorNum; ++i)
    {
        m_listColor[i] = colormap.GetColorAt(i);
    }

    return *this;
}

void CKEColormap::_UpdateColormap()
{
    if (m_mapControlPointsRgb.firstKey() != 0)
    {
        for (auto i = 0; i != m_mapControlPointsRgb.firstKey(); ++i)
        {
            m_listColor[i] = m_mapControlPointsRgb.first();
        }
    }

    // interpolation
    for (auto iter = m_mapControlPointsRgb.cbegin(); iter != m_mapControlPointsRgb.cend(); ++iter)
    {
        if (std::next(iter) == m_mapControlPointsRgb.cend())
            break;

        for (auto i = iter.key(); i != std::next(iter).key(); ++i)
        {
            float factor = (i - iter.key()) * 1.0f / (std::next(iter).key() - iter.key());

            m_listColor[i].setRed(qRed(iter.value()) + qRound((qRed(std::next(iter).value()) - qRed(iter.value())) * factor));
            m_listColor[i].setGreen(qGreen(iter.value()) + qRound((qGreen(std::next(iter).value()) - qGreen(iter.value())) * factor));
            m_listColor[i].setBlue(qBlue(iter.value()) + qRound((qBlue(std::next(iter).value()) - qBlue(iter.value())) * factor));
        }
    }

    if (m_mapControlPointsRgb.lastKey() != m_colorNum - 1)
    {
        for (auto i = m_mapControlPointsRgb.lastKey(); i != m_colorNum; ++i)
        {
            m_listColor[i] = m_mapControlPointsRgb.last();
        }
    }
    else
    {
        m_listColor[m_colorNum - 1] = m_mapControlPointsRgb.last();
    }

    // alpha
    for (auto iter = m_mapControlPointsAlpah.cbegin(); iter != m_mapControlPointsAlpah.cend(); ++iter)
    {
        m_listColor[iter.key()].setAlpha(iter.value());
    }
}

void CKEColormap::_WriteToJson(QJsonObject &colormap)
{
    colormap["number"] = m_colorNum;

    QJsonObject invalidColor
    {
        { "red", m_invalidColor.red() },
        { "green", m_invalidColor.green() },
        { "blue", m_invalidColor.blue() },
        { "alpha", m_invalidColor.alpha() }
    };

    colormap["invalid_point"] = invalidColor;

    QJsonArray arrayRgb;

    for (auto iter = m_mapControlPointsRgb.cbegin(); iter != m_mapControlPointsRgb.cend(); ++iter)
    {
        QJsonObject rgbControlPoint
        {
            { "position", iter.key() },
            { "red", qRed(iter.value()) },
            { "green", qGreen(iter.value()) },
            { "blue", qBlue(iter.value()) }

        };

        arrayRgb << rgbControlPoint;
    }

    colormap["rgb_control_points"] = arrayRgb;

    QJsonArray arrayAlpha;

    for (auto iter = m_mapControlPointsAlpah.cbegin(); iter != m_mapControlPointsAlpah.cend(); ++iter)
    {
        QJsonObject alphaControlPoint
        {
            { "position", iter.key() },
            { "alpha", iter.value() }
        };

        arrayAlpha << alphaControlPoint;
    }

    colormap["alpha_control_points"] = arrayAlpha;
}

void CKEColormap::_WriteToJson(rapidjson::Document &colormap)
{
    colormap.SetObject();
    rapidjson::Document::AllocatorType& allocator = colormap.GetAllocator();
    
    colormap.AddMember("number", m_colorNum, allocator);

    rapidjson::Value invalidColor(rapidjson::kObjectType);
    invalidColor.AddMember("red", m_invalidColor.red(), allocator);
    invalidColor.AddMember("green", m_invalidColor.green(), allocator);
    invalidColor.AddMember("blue", m_invalidColor.blue(), allocator);
    invalidColor.AddMember("alpha", m_invalidColor.alpha(), allocator);

    colormap.AddMember("invalid_point", invalidColor, allocator);

    rapidjson::Value rgbControlPoint(rapidjson::kArrayType);

    for (auto iter = m_mapControlPointsRgb.cbegin(); iter != m_mapControlPointsRgb.cend(); ++iter)
    {
        rapidjson::Value posColor(rapidjson::kObjectType);
        posColor.AddMember("position", iter.key(), allocator);
        posColor.AddMember("red", qRed(iter.value()), allocator);
        posColor.AddMember("green", qGreen(iter.value()), allocator);
        posColor.AddMember("blue", qBlue(iter.value()), allocator);

        rgbControlPoint.PushBack(posColor, allocator);
    }

    colormap.AddMember("rgb_control_points", rgbControlPoint, allocator);

    rapidjson::Value alphaControlPoint(rapidjson::kArrayType);

    for (auto iter = m_mapControlPointsAlpah.cbegin(); iter != m_mapControlPointsAlpah.cend(); ++iter)
    {
        rapidjson::Value posAlpha(rapidjson::kObjectType);
        posAlpha.AddMember("position", iter.key(), allocator);
        posAlpha.AddMember("alpha", iter.value(), allocator);

        alphaControlPoint.PushBack(posAlpha, allocator);
    }

    colormap.AddMember("alpha_control_points", alphaControlPoint, allocator);
}