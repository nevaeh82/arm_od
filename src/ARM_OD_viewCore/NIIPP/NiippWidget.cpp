#include "NIIPPWidget.h"
#include "ui_NIIPPControlWidget.h"

NiippControlWidget::NiippControlWidget(NiippController* control, QWidget *parent) :
	QWidget(parent)
	, ui(new Ui::NiippControlWidget)
	, _controller(control)
{
	ui->setupUi(this);

	ui->_pb_enable_complex->setCheckable(true);
	ui->_pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
	connect(ui->_pb_enable_complex, SIGNAL(toggled(bool)), this, SLOT(enableComplex(bool)));
	connect(ui->_pb_enable_complex, SIGNAL(toggled(bool)), this, SIGNAL(signalEnableComplex(bool)));

	QFont* font = new QFont();
	font->setBold(true);
	font->setPixelSize(20);

	ui->_sb_power->setFont(*font);

	ui->_sl_power->setRange(0, 27);
	ui->_sl_power->setTickPosition(QSlider::TicksBelow);
	ui->_sl_power->setTickInterval(4);

	connect(ui->_sl_power, SIGNAL(valueChanged(int)), ui->_sb_power, SLOT(setValue(int)));
	connect(ui->_sb_power, SIGNAL(valueChanged(int)), this, SLOT(changeValuePower(int)));
	connect(ui->_sb_power, SIGNAL(valueChanged(int)), this, SIGNAL(signalChangeValuePower(int)));

	ui->_le_distance->setFont(*font);
	ui->_le_distance->setText(tr("0 км"));
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

	connect(ui->_pb_start, SIGNAL(clicked(bool)), this, SLOT(startStopClicked(bool)));
	connect(ui->_pb_start, SIGNAL(clicked(bool)), this, SIGNAL(signalStartStopClicked(bool)));

	ui->_cb_antena->addItem(tr("Направленная"));
	ui->_cb_antena->addItem(tr("Ненапрвленная"));

	connect(ui->_cb_antena, SIGNAL(activated(int)), this, SLOT(setAntennaType(int)));
	connect(ui->_cb_antena, SIGNAL(activated(int)), this, SIGNAL(signalSetAntennaType(int)));

	ui->_cb_mode->addItem(tr("Облучение"));
	ui->_cb_mode->addItem(tr("Приведение"));
	ui->_cb_mode->addItem(tr("Круговой обзор"));
	connect(ui->_cb_mode, SIGNAL(activated(int)), this, SIGNAL(signalChangeMode(int)));

	ui->_le_lat->setFixedWidth(70);
	ui->_le_lon->setFixedWidth(70);

	ui->_le_status->setReadOnly(true);
	ui->_le_status->setText(tr("Простой"));

	connect(ui->_clear_uvod, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui->_clear_uvod, SIGNAL(clicked()), this, SIGNAL(signalClear()));
}

NiippControlWidget::~NiippControlWidget()
{
	delete ui;
}

bool NiippControlWidget::getState()
{
	return ui->_pb_enable_complex->isChecked();
}

void NiippControlWidget::enableComplex(bool state)
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

void NiippControlWidget::changeValuePower(int value)
{
	ui->_sl_power->setValue(value);

	if(ui->_cb_antena->currentIndex() == 0) {
		ui->_le_distance->setText(QString("%1 км").arg(QString::number(_controller->getRadiusSector())));
		if(ui->_pb_start->isChecked())
		{
			ui->_le_status->setText(tr("Облучение"));
		}
	}
	if(ui->_cb_antena->currentIndex() == 1) {
		ui->_le_distance->setText(QString("%1 км").arg(QString::number(_controller->getRadiusCircle())));
		if(ui->_pb_start->isChecked())
		{
			ui->_le_status->setText(tr("Облучение"));
		}
	}
}

void NiippControlWidget::startStopClicked(bool state)
{
	ui->_pb_start->setChecked(state);

	if(state == true) {
		ui->_pb_start->setStyleSheet("background-color: green;" \
								 "border-style: solid;" \
								 "border-width:1px;" \
								 "border-radius:25px;" \
								 "border-color: red;" \
								 "max-width:50px;" \
								 "max-height:50px;" \
								 "min-width:50px;"\
								 "min-height:50px");
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
	}
}

void NiippControlWidget::setAntennaType(int value)
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
			ui->_le_status->setText(tr("Облучение"));
		}
	}
	else {
		ui->_cb_mode->setEnabled(true);

		ui->_cb_mode->setCurrentIndex(_controller->getModeCurrentIndex());

		ui->_sl_power->setRange(0, 27);
		ui->_sl_power->update();
		ui->_sb_power->setRange(0, 27);
		ui->_sb_power->update();
		if(ui->_pb_start->isChecked())
		{
			ui->_le_status->setText(tr("Облучение"));
		}
	}
}

void NiippControlWidget::clear()
{
	ui->_le_lat->clear();
	ui->_le_lon->clear();
}

void NiippControlWidget::setPoint(QPointF coord)
{
	QString lat_s = QString::number(coord.x(), 'f', 4);
	ui->_le_lat->setText(lat_s);
	QString lon_s = QString::number(coord.y(), 'f', 4);
	ui->_le_lon->setText(lon_s);
}

bool NiippControlWidget::getEnableComplexState()
{
	return ui->_pb_enable_complex->isChecked();
}

bool NiippControlWidget::getStartState()
{
	return ui->_pb_start->isChecked();
}

int NiippControlWidget::getModeIndex()
{
	return ui->_cb_mode->currentIndex();
}

int NiippControlWidget::getAntenaIndex()
{
	return ui->_cb_antena->currentIndex();
}

int NiippControlWidget::getSbPowerValue()
{
	return ui->_sb_power->value();
}

void NiippControlWidget::setStatusText(int mode)
{
	switch(mode) {
	case 00:
		ui->_le_status->setText(tr("Простой"));
		break;
	case 01:
		ui->_le_status->setText(tr("Облучение"));
		break;
	case 10:
		ui->_le_status->setText(tr("Облучение"));
		break;
	case 11:
		ui->_le_status->setText(tr("Авария"));
		break;
	case 55:
		ui->_le_status->setText(tr("Нет координат"));
		break;
	case 99:
		ui->_le_status->setText(tr("Обслуживание"));
		break;
	default:
		ui->_le_status->setText(tr("Нет данных"));
		break;
	}
}

void NiippControlWidget::setLatText(QString text)
{
	ui->_le_lat->setText(text);
}

void NiippControlWidget::setLonText(QString text)
{
	ui->_le_lon->setText(text);
}
