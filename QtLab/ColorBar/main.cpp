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
    //CKEColormap::InitializeColormapsFromFile();
    //CKEColormap::InitializeColormapsFromJason("C:\\Program Files\\JasonSW\\Jason 8.4\\etc\\colorscales");
    //CKEColormap::WriteColormapsToDB();

    Dialog dialog;
    dialog.show();

    return a.exec();
}
