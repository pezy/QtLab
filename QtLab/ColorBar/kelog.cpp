#include "kelog.h"

#include <QtCore/QDateTime>
#include <QtCore/QFile>

void CKELog::AddLog(QString str)
{
    static QFile qFile("./KerogenLog.txt");

    if ( qFile.exists() ) {
        if ( !qFile.open(QIODevice::Append | QIODevice::Text) ) return;
    } else {

        if ( !qFile.open(QIODevice::WriteOnly | QIODevice::Text) ) return;
    }

    str = QDateTime::currentDateTime().toString() + ": " + str + "\n";

    qFile.write(str.toLatin1().constData());

    qFile.close();
}