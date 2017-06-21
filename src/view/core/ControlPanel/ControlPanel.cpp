#include "ControlPanel.h"
#include "ui_ControlPanel.h"

ControlPanel::ControlPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ControlPanel)
{
	ui->setupUi(this);

	connect(ui->startPB, SIGNAL(clicked()), this, SIGNAL(startPlayingHistorySignal()));
	connect(ui->stopPB, SIGNAL(clicked()), this, SIGNAL(stopPlayingHistorySignal()));
	connect(ui->pbExport, SIGNAL(clicked()), this, SIGNAL(startExportToXls()));

	connect(ui->spinBoxPlaySpeed, SIGNAL(valueChanged(int)), this, SIGNAL(signalOnMux(int)));

	ui->pbExport->setVisible(false);

	ui->timeLabel->hide();
	ui->timeText->hide();

	bool status = false;
	status &= greenLbl.load(":/images/bullet_green.png");
	status &= redLbl.load(":/images/bullet_red.png");
	status &= yellowLbl.load(":/images/bullet_yellow.png");

	ui->dockWidget->hide();

	ui->connectToArmRlbl->setPixmap(redLbl);
	ui->connectToDbLbl->setPixmap(redLbl);
	ui->lblWriteToBd->setPixmap(yellowLbl);
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
	// saving selection before change text
	int selStart = ui->timeText->selectionStart();
	int selLength = ui->timeText->selectedText().length();

	ui->timeText->setText( value.toString( Qt::SystemLocaleShortDate ) );

	// restore selection
	if( selStart > -1 ) {
		ui->timeText->setSelection( selStart, selLength );
	}
}

void ControlPanel::setPlayingEnabled(bool enabled)
{
	ui->startTE->setEnabled( !enabled );
	ui->stopTE->setEnabled( !enabled );
	ui->stopPB->setEnabled( enabled );
	ui->timeLabel->setVisible( enabled );
	ui->timeText->setVisible( enabled );

	ui->startPB->setText( enabled ? tr( "Pause") : tr( "Start") );
}

void ControlPanel::setPause()
{
	ui->startPB->setText( tr( "Resume") );
}

void ControlPanel::setDataBaseStatus(bool isOpened)
{
	if(isOpened) {
		ui->connectToDbLbl->setPixmap(greenLbl);
	} else {
		ui->connectToDbLbl->setPixmap(redLbl);
	}
}

QDockWidget*ControlPanel::getDockWgt()
{
	return ui->dockWidget;
}

void ControlPanel::onSetDbLog(const QString& logTxt)
{
	//ui->textEdit->clear();
	ui->textEdit->append(logTxt);
}

void ControlPanel::setARMRConnection(bool b)
{
	if(b) {
		ui->connectToArmRlbl->setPixmap(greenLbl);
	} else {
		ui->connectToArmRlbl->setPixmap(redLbl);
	}
}

void ControlPanel::setBdWriteState(int val)
{
	if(val < 0) {
		ui->lblWriteToBd->setPixmap(redLbl);
	} else if(val == 0) {
		ui->lblWriteToBd->setPixmap(yellowLbl);
	} else {
		ui->lblWriteToBd->setPixmap(greenLbl);
	}
}
