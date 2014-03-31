#include "ControlPanel.h"
#include "ui_ControlPanel.h"

ControlPanel::ControlPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ControlPanel)
{
	ui->setupUi(this);

	connect(ui->blaPB, SIGNAL(clicked()), this, SIGNAL(showBlaClicked()));
	connect(ui->bplaPB, SIGNAL(clicked()), this, SIGNAL(showBplaClicked()));
	connect(ui->spipDDPB, SIGNAL(clicked()), this, SIGNAL(showNiippClicked()));
}

ControlPanel::~ControlPanel()
{
	delete ui;
}
