#include "workbench.h"
#include "products.h"

#include <QIcon>
#include <QGridLayout>
#include <QPushButton>
#include <QProcess>
#include <QLabel>

WorkBench::WorkBench(QWidget *parent)
	: QWidget(parent)
{
	setWindowIcon(QIcon(":/WorkBench/logo"));
	setWindowTitle("Qt Workbench");

	// style sheet cannot work...
	//setStyleSheet(QStringLiteral("background-image: url(:/WorkBench/Resources/TheQtCompany_logo_1200x630.png);"));
	//setStyleSheet(QStringLiteral("background-color:black;"));

	QPalette pal(palette());
	pal.setBrush(backgroundRole(), QBrush(QImage(":/WorkBench/backgroud")));
	setPalette(pal);

	setFixedSize(1200, 630);

	_Init();
	_CheckLicense();
}

WorkBench::~WorkBench()
{

}

void WorkBench::keyPressEvent(QKeyEvent * e)
{
}

void WorkBench::_Init()
{
	const int cnWidth = 32;
	const int cnHeight = 32;

	const QList<Products::SProducts>& listProducts = Products::Instance().GetAllProducts();

	auto pLayoutWidget = new QWidget(this);
	pLayoutWidget->setGeometry(QRect(30, 30, (cnWidth + 40) * listProducts.size(), cnHeight + 75));

	auto pLayout = new QGridLayout(pLayoutWidget);

	for (int i = 0; i != listProducts.size(); ++i)
	{
		auto pButton = new QPushButton(this);
		pButton->setFixedSize(cnWidth, cnHeight);
		connect(pButton, &QPushButton::pressed, this, &WorkBench::slotProcess);
		m_group.addButton(pButton, i);

		auto pLabel = new QLabel(this);
		pLabel->setAlignment(Qt::AlignHCenter);
		pLabel->setFixedWidth(cnWidth);
		pLabel->setStyleSheet("font: 75 12pt \"Georgia\";\ncolor: rgb(255, 255, 255);");
		pLabel->setText(listProducts[i].m_name);

		pLayout->addWidget(pButton, 0, i);
		pLayout->addWidget(pLabel, 1, i);
	}

	pLayoutWidget->setLayout(pLayout);
}

void WorkBench::_CheckLicense()
{
	const QList<Products::SProducts>& listVersion = Products::Instance().GetAllProducts();
	QList<bool> listCheckResult{ true, false, true, true };

	for (int i = 0; i != listCheckResult.size(); ++i)
	{
		m_group.button(i)->setEnabled(listCheckResult[i]);
		QString strIcon = listCheckResult[i] ? listVersion[i].m_icon : listVersion[i].m_iconDisabled;
		m_group.button(i)->setStyleSheet("background-image: url(:/appicon/" + strIcon + ");");
	}
}

void WorkBench::slotProcess()
{
	auto pButton = dynamic_cast<QPushButton*>(sender());
	if (!pButton) return;

	const QList<Products::SProducts>& listVersion = Products::Instance().GetAllProducts();
	
	auto pProcess = new QProcess(this);
	pProcess->start(listVersion.at(m_group.id(pButton)).m_name + ".exe");
}
