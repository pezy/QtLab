#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include "tabwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *widget1 = new QWidget;
    QVBoxLayout *layout1 = new QVBoxLayout(widget1);
    layout1->addWidget(new QLabel("Inform"));

    QWidget *widget2 = new QWidget;
    QVBoxLayout *layout2 = new QVBoxLayout(widget2);
    layout2->addWidget(new QLabel("Switch"));

    TabWidget tabWidget;
    tabWidget.addTab(widget1, QIcon(":/current_inform"), "");
    tabWidget.addTab(widget2, QIcon(":/switch"), "");
    tabWidget.show();

    return app.exec();
}
