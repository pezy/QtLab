#include <QtWidgets/QApplication>
#include "kecolormapbar.h"
#include "kecolormap.h"
#include "kedbio.h"
#include "kesaveload.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CKEColormap::InitializeColormapsFromDB();

    Dialog dialog;
    dialog.show();

    /*CKEDBIO *pDBIO = new CKEDBIO;
    QString strDBFile = CKEDBIO::GetDefaultDBFileName();
    pDBIO->ConnectDB(strDBFile);
    
    CKEColormap::InitializeColormaps();
    QStringList listName = CKEColormap::GetAllColormapsName();
    QStringList listField{"ID", "ColormapData"};
    
    pDBIO->Transaction();
    for (int i = 0; i < listName.size(); ++i)
    {
        QList<QVariant> vtList;

        const int nColorSize = 256;
        unsigned int *pData = new unsigned int[nColorSize];

        CKEColormap *pColormap = CKEColormap::GetColormap(listName.at(i));
        for (int j = 0; j < nColorSize; ++j)
        {
            QColor color = pColormap->GetColorAt(j);
            pData[j] = color.rgb();
        }

        vtList.append(listName.at(i));
        vtList.append(SaveDataToBLOB(pData, nColorSize));

        pDBIO->Insert("Colormap", listField, vtList);
    }
    pDBIO->Commit();*/

    return a.exec();
}
