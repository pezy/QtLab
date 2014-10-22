#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	Widget widget;
	widget.show();

    return app.exec();
}
