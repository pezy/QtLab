#include "customtable.h"
#include <QCheckBox>
#include <QComboBox>

CustomTable::CustomTable(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	auto pCheckWidget = new QWidget();
	auto pHorizonLayout = new QHBoxLayout();
	pHorizonLayout->setAlignment(Qt::AlignCenter);
	pHorizonLayout->addWidget(new QCheckBox());
	pCheckWidget->setLayout(pHorizonLayout);

	auto pComboBox = new QComboBox();
	pComboBox->addItems({"First", "Second"});

	ui.tableWidget->setRowCount(1);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(35);

	ui.tableWidget->setCellWidget(0, 0, pComboBox);
	ui.tableWidget->setCellWidget(0, 1, pCheckWidget);
}

CustomTable::~CustomTable()
{

}
