#include "tabwidget.h"
#include <QWidget>

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setStyleSheet("QTabBar::tab {margin:  1px 1px 1px 1px; background: rgb(240,240,240);}");
    setIconSize(QSize(100,49));

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(iconChanged(int)));
    resize(QSize(400, 100));
}


void TabWidget::iconChanged(int id) {
    switch (id)
    {
    case 0:
        setTabIcon(0, QIcon(":/current_inform"));
        setTabIcon(1, QIcon(":/switch"));
        break;
    case 1:
        setTabIcon(0, QIcon(":/inform"));
        setTabIcon(1, QIcon(":/current_switch"));
        break;
    }
}
