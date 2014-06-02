#include "SolverSettingsDialog.h"
#include "ui_SolverSettingsDialog.h"

SolverSettingsDialog::SolverSettingsDialog(QWidget *parent) :
	QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint),
	ui(new Ui::SolverSettingsDialog)
{
	ui->setupUi(this);

	setWindowIconText(tr("Detector"));
	setWindowTitle(tr("Solver settings"));

	ui->checkBoxAuto->setCheckState(Qt::Checked);

	ui->AltitudeSpinBox->setValue(0);
	ui->AltitudeSpinBox->setRange(0, 10000);

	ui->TrackLenSpinBox->setRange(0, 1000);
	ui->TrackLenSpinBox->setValue(0);

	ui->SamplesCountSpinBox->setRange(0, 1000);
	ui->SamplesCountSpinBox->setValue(0);

	connect(ui->ClearButton, SIGNAL(clicked()), this, SIGNAL(signalClear()));
	connect(ui->AcceptButton, SIGNAL(clicked()), this, SIGNAL(signalAccept()));
	connect(ui->CancelButton, SIGNAL(clicked()), this, SIGNAL(signalCancel()));
	connect(ui->checkBoxAuto, SIGNAL(toggled(bool)), this, SIGNAL(signalAuto(bool)));

	connect(ui->checkBoxAuto, SIGNAL(toggled(bool)), this, SLOT(autoStateToggled(bool)));
}

SolverSettingsDialog::~SolverSettingsDialog()
{
	delete ui;
}

int SolverSettingsDialog::getTrackLength()
{
	return ui->TrackLenSpinBox->value();
}

double SolverSettingsDialog::getHeight()
{
	return ui->AltitudeSpinBox->value();
}

int SolverSettingsDialog::getCountNum()
{
	return ui->SamplesCountSpinBox->value();
}

bool SolverSettingsDialog::getAutoState()
{
	return ui->checkBoxAuto->isChecked();
}

void SolverSettingsDialog::setTrackLength(int val)
{
	ui->TrackLenSpinBox->setValue(val);
}

void SolverSettingsDialog::setHeight(double val)
{
	ui->AltitudeSpinBox->setValue(val);
}

void SolverSettingsDialog::setCountNum(int val)
{
	ui->SamplesCountSpinBox->setValue(val);
}

void SolverSettingsDialog::setAutoState(bool val)
{
	ui->checkBoxAuto->setChecked(val);
}

void SolverSettingsDialog::autoStateToggled(bool val)
{
	ui->TrackLenSpinBox->setDisabled(val);
	ui->AltitudeSpinBox->setDisabled(val);
	ui->SamplesCountSpinBox->setDisabled(val);
}
