#include "kedbio.h"
#include <qsqldatabase.h>
#include <qfile.h>
#include <qdir.h>
#include <qsqlerror.h>
#include <qcoreapplication.h>
#include <QSqlRecord>
#include <QSqlField>


CKEDBIO::CKEDBIO()
{
	m_pDB = NULL;
}

CKEDBIO::~CKEDBIO()
{
	Close();
}

QString CKEDBIO::_GetDBFileName(const QString& strDBName)
{
	QDir dir( QCoreApplication::applicationFilePath() );

	while(dir.cdUp())
	{
		QString strFileName = dir.absolutePath() + QDir::separator() + strDBName;
		if (QFile::exists(strFileName))
			return strFileName;

		QStringList strDirList = dir.entryList(QDir::Dirs);		

		if (strDirList.contains("Kerogen"))
		{
			QString strPath = dir.absolutePath();
			return strPath + QDir::separator() + "Kerogen" + QDir::separator() + strDBName;
		}
	}

	return "";

}

QString CKEDBIO::GetDefaultDBFileName()
{
	QString strDBName = "Kerogen.tdb";
	return _GetDBFileName(strDBName);
}

QString CKEDBIO::GetTemplateFileName()
{
	QString strDBName = "Templates.xmt";
	return _GetDBFileName(strDBName);
}

QString CKEDBIO::GetColormapFilePath()
{
	QString strColorMapDir = "Colormap";
	QDir dir( QCoreApplication::applicationFilePath() );
	while(dir.cdUp())
	{
		QStringList strDirList = dir.entryList(QDir::Dirs);
		if (strDirList.contains(strColorMapDir))
		{
			QString strFileName = dir.absolutePath();
			return strFileName + QDir::separator() + strColorMapDir + QDir::separator();
		}
	}

	return "";
}

QString CKEDBIO::GetShaderFilePath()
{
	QString strColorMapDir = "shader";
	QDir dir( QCoreApplication::applicationFilePath() );
	while(dir.cdUp())
	{
		QStringList strDirList = dir.entryList(QDir::Dirs);
		if (strDirList.contains(strColorMapDir))
		{
			QString strFileName = dir.absolutePath();
			return strFileName + QDir::separator() + strColorMapDir + QDir::separator();
		}
	}

	return "";
}

void CKEDBIO::_NewDB()
{
	if (!m_pDB)
	{
		m_pDB = new QSqlDatabase();
	}
}

void CKEDBIO::Close()
{
	if (m_pDB)
	{
		m_pDB->commit();
		m_pDB->close();
		delete m_pDB;
		m_pDB = NULL;
	}
}

bool CKEDBIO::ConnectDB(const QString& strFullDBName, bool bInMemory)
{
	_NewDB();

	bool bSuccess = m_pDB->isOpen();
	if ( !bSuccess )
	{
		if (! bInMemory)
		{
			// Open database
			if ( QFile::exists (strFullDBName ) )
			{
				QFile::setPermissions(strFullDBName,(QFile::Permissions)0x7777);
				*m_pDB = QSqlDatabase::addDatabase ("QSQLITE",::GetUniqueGuid().toString() ) ;
				m_pDB->setDatabaseName ( strFullDBName  ) ;
				bSuccess = m_pDB->open () ;
			}
		}
		else
		{
			*m_pDB = QSqlDatabase::addDatabase ("QSQLITE",::GetUniqueGuid().toString() ) ;
			m_pDB->setDatabaseName ( ":memory:"  ) ;
			bSuccess = m_pDB->open () ;
		}
	}

	return bSuccess;
}

bool CKEDBIO::UpdateInsert(const QString& strTable, const QStringList& strFieldList, const QList<QVariant>& valueList, const QString& strKey)
{
	if (!Update(strTable, strFieldList, valueList, strKey))
		return Insert(strTable, strFieldList, valueList);
	else
		return true;
}

bool CKEDBIO::UpdateInsert(const QString& strTable, const QStringList& strFieldList, const QList<QVariant>& valueList, const QStringList& strListKeyName, const QStringList& strListKeyValue)
{
	if (!Update(strTable, strFieldList, valueList, strListKeyName, strListKeyValue))
		return Insert(strTable, strFieldList, valueList);
	else
		return true;
}

bool CKEDBIO::Update(const QString& strTable, const QStringList& strFieldList, const QList<QVariant>& valueList, const QString& strKey)
{
	QString strSql;

	// set fields
	int nSize = strFieldList.size();
	for (int i=0; i<nSize; ++i)
	{
		if (i != 0)
		{
			strSql += ", ";
		} 

		strSql += (strFieldList.at(i) + "=?");
	}

	// add where
	QString strWhere(" WHERE ");
	strWhere += "ID = '";
	strWhere += strKey;
	strWhere += "'";


	// sql
	strSql = "UPDATE " + strTable + " SET " + strSql + strWhere;

	// query
	bool bSuccess;
	QSqlQuery query( *m_pDB  );
	{
		query.prepare(strSql);

		for (int i=0; i<nSize; ++i)
		{
			query.addBindValue(valueList.at(i));
		}

		query.exec();
	}

	// deal with return value
	bSuccess = (query.numRowsAffected()>=1 );

	return bSuccess;
}

bool CKEDBIO::Update(const QString& strTable, const QStringList& strFieldList, const QList<QVariant>& valueList, const QStringList& strListKeyName, const QStringList& strListKeyValue)
{
	QString strSql;

	// set fields
	int nSize = strFieldList.size();
	for (int i=0; i<nSize; ++i)
	{
		if (i != 0)
		{
			strSql += ", ";
		} 

		strSql += (strFieldList.at(i) + "=?");
	}

	// add where
	QString strWhere(" WHERE ");
	nSize = strListKeyName.size();
	for (int i=0; i<nSize; ++i)
	{
		strWhere += strListKeyName.at(i);
		strWhere += " = '";
		strWhere += strListKeyValue.at(i);
		strWhere += "'";

		if (i != nSize-1)
		{
			strWhere += " AND ";
		}
	}

	// sql
	strSql = "UPDATE " + strTable + " SET " + strSql + strWhere;

	// query
	bool bSuccess;
	QSqlQuery query( *m_pDB  );
	{
		query.prepare(strSql);

		nSize = strFieldList.size();
		for (int i=0; i<nSize; ++i)
		{
			query.addBindValue(valueList.at(i));
		}

		query.exec();
	}

	// deal with return value
	bSuccess = (query.numRowsAffected()>=1 );

	return bSuccess;
}

bool CKEDBIO::Insert(const QString& strTable, const QStringList& strFieldList, const QList<QVariant>& valueList)
{
	QString strSql("(");

	// set fields
	int nSize = strFieldList.size();
	for (int i=0; i<nSize; ++i)
	{
		strSql += strFieldList.at(i);
		if (i != nSize -1)
			strSql += ", ";
	}

	strSql += ") VALUES (";
	for (int i=0; i<nSize; ++i)
	{
		strSql += "?";
		if (i != nSize -1)
			strSql += ", ";
	}
	strSql += ")";

	// sql
	strSql = "INSERT INTO " + strTable + " " + strSql;

	// query
	bool bSuccess;
	QSqlQuery query( *m_pDB  );
	{
		query.prepare(strSql);

		for (int i=0; i<nSize; ++i)
		{
			query.addBindValue(valueList.at(i));
		}

		query.exec();
	}

	// deal with return value
	bSuccess = (query.numRowsAffected()>=1 );

	return bSuccess;
}

QSqlQuery CKEDBIO::Select(const QString& strTable, const QStringList& strFieldList, const QString& strKey)
{
	//m_pDB->transaction();

	QString strSql;

	// set fields
	int nSize = strFieldList.size();
	for (int i=0; i<nSize; ++i)
	{
		strSql += strFieldList.at(i);

		if (i != nSize -1)
			strSql += ", ";
	}

	// add where
	QString strWhere(" WHERE ID= '");
	strWhere += strKey;
	strWhere += "'";

	// sql
	strSql = "Select " + strSql + " FROM " + strTable + strWhere;

	// Select
	bool bSuccess;
	QSqlQuery query(*m_pDB );
	{
		query.prepare(strSql);
		bSuccess = query.exec();
	}

	if (!bSuccess)
	{
		QSqlError sqlError = query.lastError();
	}
		
	// return query object 
	return query;
}

QSqlQuery CKEDBIO::Select(const QString& strTable, const QStringList& strFieldList, const QStringList& strListKeyName, const QStringList& strListKeyValue)
{
	//m_pDB->transaction();

	QString strSql;

	// set fields
	int nSize = strFieldList.size();
	for (int i=0; i<nSize; ++i)
	{
		strSql += strFieldList.at(i);

		if (i != nSize -1)
			strSql += ", ";
	}

	// add where
	QString strWhere(" WHERE ");
	nSize = strListKeyName.size();
	for (int i=0; i<nSize; ++i)
	{
		strWhere += strListKeyName.at(i);
		strWhere += " = '";
		strWhere += strListKeyValue.at(i);
		strWhere += "'";

		if (i != nSize-1)
		{
			strWhere += " AND ";
		}
	}

	// sql
	strSql = "Select " + strSql + " FROM " + strTable + strWhere;

	// Select
	bool bSuccess;
	QSqlQuery query(*m_pDB );
	{
		query.prepare(strSql);
		bSuccess = query.exec();
	}

	if (!bSuccess)
	{
		QSqlError sqlError = query.lastError();
	}
		
	// return query object 
	return query;
}

QString CKEDBIO::GetProjectID()
{
	QString strSql = "Select ID FROM rlproject";

	QSqlQuery query( *m_pDB  );
	{
		query.prepare(strSql);
		query.exec();
	}

	if( query.first() )
	{
		int nIndex = -1;

		return query.value( ++nIndex ).toString();
	}

	return "";
}

bool CKEDBIO::Transaction()
{
	if (m_pDB)
		return m_pDB->transaction();
	else
		return false;
}

void CKEDBIO::Commit()
{
	if (m_pDB)
	{
		m_pDB->commit();
	}
}

void CKEDBIO::Delete(const QString& strTable, const QString& strKey)
{
	// add where
	QString strWhere(" WHERE ID= '");
	strWhere += strKey;
	strWhere += "'";

	QString strSql = "DELETE FROM " + strTable + strWhere;

	QSqlQuery query( *m_pDB  );
	{
		query.prepare(strSql);
		query.exec();
	}	
}

void CKEDBIO::Delete(const QString& strKey)
{
	QString strSql = "SELECT name FROM sqlite_master WHERE type='table'";
	
	QSqlQuery query( *m_pDB  );
	{
		query.prepare(strSql);
		query.exec();
	}

	while(query.next())
	{
		int nIndex = -1;

		Delete(query.value(++nIndex).toString(), strKey);
	}
}

void CKEDBIO::MergeDB(const QString& strProjectDBFileName)
{
	// project database
	if (!QFile::exists(strProjectDBFileName))
		return;

	QFile::setPermissions(strProjectDBFileName, (QFile::Permissions)0x7777);
	QSqlDatabase db_project = QSqlDatabase::addDatabase("QSQLITE", ::GetUniqueGuid().toString());
	db_project.setDatabaseName(strProjectDBFileName);
	if (!db_project.open())
		return;

	// template database
	QString strTemplateDBFileName = GetDefaultDBFileName();
	if (!QFile::exists(strTemplateDBFileName))
		return;

	QSqlDatabase db_template = QSqlDatabase::addDatabase("QSQLITE", ::GetUniqueGuid().toString());
	db_template.setDatabaseName(strTemplateDBFileName);
	if (!db_template.open())
		return;

	// merge new table
	QStringList listTemTable = db_template.tables();
	QStringList listProTable = db_project.tables();

	for (int i = 0; i < listTemTable.size(); ++i)
	{
		// copy table
		if (!listProTable.contains(listTemTable[i]))
		{
			//attach database
			QString strSql = QString("ATTACH DATABASE '") + strTemplateDBFileName + "' AS " + "'tempDB'";
			QSqlQuery query(db_project);
			query.prepare(strSql);
			query.exec();

			// create table
			strSql = QString("CREATE TABLE ") + listTemTable[i] + " AS SELECT * FROM tempDB." + listTemTable[i];
			query.prepare(strSql);
			query.exec();
		}

		QSqlRecord recordTem = db_template.record(listTemTable[i]);
		QSqlRecord recordPro = db_project.record(listTemTable[i]);

		if (recordTem.count() <= recordPro.count())
			continue;

		QStringList listFieldTem, listFieldPro;
		for (int ii = 0; ii < recordTem.count(); ++ii)
			listFieldTem.append(recordTem.fieldName(ii));

		for (int ii = 0; ii < recordPro.count(); ++ii)
			listFieldPro.append(recordPro.fieldName(ii));

		std::vector<QSqlField> vecField;
		for (int ii = 0; ii < listFieldTem.size(); ++ii)
		{
			if (listFieldPro.contains(listFieldTem[ii]))
				continue;

			vecField.push_back(recordTem.field(ii));
		}

		// add columns
		for (int j = 0; j < vecField.size(); ++j)
		{
			QString strSql = QString("ALTER TABLE ") + listTemTable.at(i) + " ADD " + vecField[j].name() + " TEXT DEFAULT ";
			strSql = strSql + " '" + vecField[j].defaultValue().toString() + "'";

			QSqlQuery query(db_project);
			query.prepare(strSql);
			query.exec();
		}
	}

	db_project.close();
	db_template.close();
}
