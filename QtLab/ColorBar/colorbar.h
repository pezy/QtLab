#ifndef COLORBAR_H
#define COLORBAR_H

#include <QtWidgets/QMainWindow>
#include "ui_colorbar.h"

class ColorBar : public QMainWindow
{
    Q_OBJECT

public:
    ColorBar(QWidget *parent = 0);
    ~ColorBar();

private:
    Ui::ColorBarClass ui;
};

#endif // COLORBAR_H
