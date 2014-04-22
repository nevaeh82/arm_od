#include "ControlPanel.h"
#include "ui_ControlPanel.h"

ControlPanel::ControlPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ControlPanel)
{
	ui->setupUi(this);

	connect(ui->startPB, SIGNAL(clicked()), this, SIGNAL(startPlayingHistorySignal()));
	connect(ui->stopPB, SIGNAL(clicked()), this, SIGNAL(stopPlayingHistorySignal()));
}

ControlPanel::~ControlPanel()
{
	delete ui;
}

QDateTime ControlPanel::getStartDateTime() const
{
	return ui->startTE->dateTime();
}

QDateTime ControlPanel::getEndDateTime() const
{
	return ui->stopTE->dateTime();
}

void ControlPanel::setCurrentDateTime(const QDateTime& value)
{
	ui->currentTE->setDateTime(value);
}
