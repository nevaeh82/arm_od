#include "NiippWidget.h"
#include "ui_NiippWidget.h"

NiippWidget::NiippWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::NiippWidget)
{
	ui->setupUi(this);

	ui->_pb_enable_complex->setCheckable(true);
	ui->_pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
	connect(ui->_pb_enable_complex, SIGNAL(toggled(bool)), this, SLOT(enableComplex(bool)));
	connect(ui->_pb_enable_complex, SIGNAL(toggled(bool)), this, SIGNAL(complexEnabled(bool)));

	QFont* font = new QFont();
	font->setBold(true);
	font->setPixelSize(20);

	ui->_sb_power->setFont(*font);

	ui->_sl_power->setRange(0, 27);
	ui->_sl_power->setTickPosition(QSlider::TicksBelow);
	ui->_sl_power->setTickInterval(4);

	connect(ui->_sl_power, SIGNAL(valueChanged(int)), ui->_sb_power, SLOT(setValue(int)));
	connect(ui->_sb_power, SIGNAL(valueChanged(int)), this, SIGNAL(valuePowerChanged(int)));

	ui->_le_distance->setFont(*font);
	ui->_le_distance->setText(tr("0 km"));
	ui->_le_distance->setFixedWidth(70);
	ui->_le_distance->show();

	ui->_pb_start->setStyleSheet("background-color: red;" \
							 "border-style: solid;" \
							 "border-width:1px;" \
							 "border-radius:25px;" \
							 "border-color: red;" \
							 "max-width:50px;" \
							 "max-height:50px;" \
							 "min-width:50px;"\
							 "min-height:50px");
	ui->_pb_start->setCheckable(true);

	connect(ui->_pb_start, SIGNAL(clicked(bool)), this, SLOT(start(bool)));

	connect(ui->_cb_antena, SIGNAL(activated(int)), this, SIGNAL(antennaTypeChanged(int)));

	connect(ui->_cb_mode, SIGNAL(activated(int)), this, SIGNAL(modeChanged(int)));

	ui->_le_lat->setFixedWidth(70);
	ui->_le_lon->setFixedWidth(70);

	ui->_le_status->setReadOnly(true);
	ui->_le_status->setText(tr("Simple"));

	connect(ui->_clear_uvod, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui->_clear_uvod, SIGNAL(clicked()), this, SIGNAL(cleared()));
}

NiippWidget::~NiippWidget()
{
	delete ui;
}

bool NiippWidget::getState()
{
	return ui->_pb_enable_complex->isChecked();
}

void NiippWidget::enableComplex(bool state)
{
	if(state)
	{
		ui->_pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);"));
	}
	else
	{
		ui->_pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
	}
}

void NiippWidget::changeValuePower(int value, double radiusCircle, double radiusSector, int antennaType)
{
	ui->_sl_power->setValue(value);

	if( antennaType == 0 ) {
		ui->_le_distance->setText( QString(tr("%1 km")).arg(QString::number(radiusSector)) );
		if(ui->_pb_start->isChecked()) {
			ui->_le_status->setText( tr("Radiation") );
		}
	}
	if( antennaType == 1 ) {
		ui->_le_distance->setText( QString(tr("%1 km")).arg(QString::number(radiusCircle)) );
		if(ui->_pb_start->isChecked()) {
			ui->_le_status->setText( tr("Radiation") );
		}
	}
}

void NiippWidget::start(bool state)
{
	ui->_pb_start->setChecked(state);

	if(state) {
		ui->_pb_start->setStyleSheet("background-color: green;" \
							 "border-style: solid;" \
							 "border-width:1px;" \
							 "border-radius:25px;" \
							 "border-color: red;" \
							 "max-width:50px;" \
							 "max-height:50px;" \
							 "min-width:50px;"\
							 "min-height:50px");

		emit startClicked();
	}
	else {
		ui->_pb_start->setStyleSheet("background-color: red;" \
							 "border-style: solid;" \
							 "border-width:1px;" \
							 "border-radius:25px;" \
							 "border-color: red;" \
							 "max-width:50px;" \
							 "max-height:50px;" \
							 "min-width:50px;"\
							 "min-height:50px");

		emit stopClicked();
	}
}

void NiippWidget::setAntennaType(int value, Niipp::WorkMode workMode)
{
	if(value == 1) {
		ui->_cb_mode->setEnabled(false);
		ui->_cb_mode->setCurrentIndex(-1);
		ui->_sl_power->setRange(0, 23);
		ui->_sl_power->update();
		ui->_sb_power->setRange(0, 23);
		ui->_sb_power->update();
		if(ui->_pb_start->isChecked())
		{
			ui->_le_status->setText(tr("Radiation"));
		}
	}
	else {
		ui->_cb_mode->setEnabled(true);

		ui->_cb_mode->setCurrentIndex( (int)workMode );

		ui->_sl_power->setRange(0, 27);
		ui->_sl_power->update();
		ui->_sb_power->setRange(0, 27);
		ui->_sb_power->update();
		if(ui->_pb_start->isChecked())
		{
			ui->_le_status->setText(tr("Radiation"));
		}
	}
}

void NiippWidget::clear()
{
	ui->_le_lat->clear();
	ui->_le_lon->clear();
}

void NiippWidget::setPoint(QPointF coord)
{
	QString lat_s = QString::number(coord.x(), 'f', 4);
	ui->_le_lat->setText(lat_s);
	QString lon_s = QString::number(coord.y(), 'f', 4);
	ui->_le_lon->setText(lon_s);
}

bool NiippWidget::getEnableComplexState()
{
	return ui->_pb_enable_complex->isChecked();
}

bool NiippWidget::getStartState()
{
	return ui->_pb_start->isChecked();
}

int NiippWidget::getModeIndex()
{
	return ui->_cb_mode->currentIndex();
}

int NiippWidget::getAntenaIndex()
{
	return ui->_cb_antena->currentIndex();
}

int NiippWidget::getSbPowerValue()
{
	return ui->_sb_power->value();
}

void NiippWidget::setStatusText(int mode)
{
	switch(mode) {
		case 00:
			ui->_le_status->setText(tr("Simple"));
			break;
		case 01:
			ui->_le_status->setText(tr("Radiation"));
			break;
		case 10:
			ui->_le_status->setText(tr("Radiation"));
			break;
		case 11:
			ui->_le_status->setText(tr("Accident"));
			break;
		case 55:
			ui->_le_status->setText(tr("No coordinates"));
			break;
		case 99:
			ui->_le_status->setText(tr("Service"));
			break;
		default:
			ui->_le_status->setText(tr("No data"));
			break;
	}
}

void NiippWidget::setLatText(QString text)
{
	ui->_le_lat->setText(text);
}

void NiippWidget::setLonText(QString text)
{
	ui->_le_lon->setText(text);
}
