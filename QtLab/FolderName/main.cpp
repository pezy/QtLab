#include <QDir>
#include <iostream>

// Codes for change LeetCode folder name.
int main()
{
    QDir dir("C:/Users/pezy/Codes");
    dir.setFilter(QDir::Dirs);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString fileName = fileInfo.fileName();
        if (fileName.size() < 3) continue;
        QChar firstChar = fileName.at(0);
        QChar thirdChar = fileName.at(2);
        if (firstChar.isDigit() && !thirdChar.isDigit() && firstChar != '0') {
            QString newName = "0"+fileName;
            dir.rename(fileName, newName);
            std::cout << qPrintable(newName) << std::endl;
        } else std::cout << qPrintable(fileName) << std::endl;
    }
}
