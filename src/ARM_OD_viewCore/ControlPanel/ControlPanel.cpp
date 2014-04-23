#include "ControlPanel.h"
#include "ui_ControlPanel.h"

ControlPanel::ControlPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ControlPanel)
{
	ui->setupUi(this);

	connect(ui->startPB, SIGNAL(clicked()), this, SIGNAL(startPlayingHistorySignal()));
	connect(ui->stopPB, SIGNAL(clicked()), this, SIGNAL(stopPlayingHistorySignal()));

	ui->currentTimeLabel->hide();
}

ControlPanel::~ControlPanel()
{
	delete ui;
}

void ControlPanel::setStartDateTime(const QDateTime& date)
{
	ui->startTE->setDateTime(date);
}

void ControlPanel::setEndDateTime(const QDateTime& date)
{
	ui->stopTE->setDateTime(date);
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
	ui->currentTimeLabel->setText( tr( "Current time: %1" )
								   .arg( value.toString( Qt::SystemLocaleShortDate ) ));
}

void ControlPanel::setPlayingEnabled(bool enabled)
{
	ui->startTE->setEnabled( !enabled );
	ui->stopTE->setEnabled( !enabled );
	ui->startPB->setEnabled( !enabled );
	ui->stopPB->setEnabled( enabled );
	ui->currentTimeLabel->setVisible( enabled );
}
