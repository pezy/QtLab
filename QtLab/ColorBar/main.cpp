#include <QtWidgets/QApplication>
#include "kecolormapbar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CKEColormapBar w;
    w.show();
    return a.exec();
}
