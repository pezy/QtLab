#include "colorbar.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorBar w;
    w.show();
    return a.exec();
}
