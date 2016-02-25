#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.m_btnReset, SIGNAL(pressed()), ui.m_colorBar, SLOT(slotReset()));
    connect(ui.m_btnFlip, SIGNAL(pressed()), ui.m_colorBar, SLOT(slotFlip()));
    connect(ui.m_colorBar, SIGNAL(ShowMsg(const QString&)), ui.m_state, SLOT(setText(const QString&)));
    connect(ui.m_cbbName, SIGNAL(currentIndexChanged(const QString &)), ui.m_colorBar, SLOT(slotTemplateChanged(const QString&)));
	connect(ui.m_btnOpacityFreeDrawing, SIGNAL(toggled(bool)), ui.m_colorBar, SLOT(slotSetOpacityFreeDrawing(bool)));

    ui.m_cbbName->setCurrentIndex(0);
}

Dialog::~Dialog()
{

}
