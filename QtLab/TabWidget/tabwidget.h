#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

class QWidget;
class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    void iconChanged(int id);
};

#endif // TABWIDGET_H
