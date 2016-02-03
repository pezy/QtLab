#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.m_btnReset, SIGNAL(pressed()), ui.m_colorBar, SLOT(slotReset()));
    connect(ui.m_btnFlip, SIGNAL(pressed()), ui.m_colorBar, SLOT(slotFlip()));
}

Dialog::~Dialog()
{

}
