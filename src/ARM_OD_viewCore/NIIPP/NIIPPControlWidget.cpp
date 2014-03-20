#include "NIIPPControlWidget.h"
#include "ui_NIIPPControlWidget.h"

NIIPPControlWidget::NIIPPControlWidget(NIIPPControl* control, QWidget *parent) :
	QWidget(parent)
	, ui(new Ui::NIIPPControlWidget)
  , _controller(control)
{
	ui->setupUi(this);

	ui->_pb_enable_complex->setCheckable(true);
	ui->_pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
	connect(ui->_pb_enable_complex, SIGNAL(toggled(bool)), this, SLOT(_slot_enable_complex(bool)));
	connect(ui->_pb_enable_complex, SIGNAL(toggled(bool)), _controller, SLOT(_slot_enable_complex(bool)));

	QFont* font = new QFont();
	font->setBold(true);
	font->setPixelSize(20);

	ui->_sb_power->setFont(*font);

	ui->_sl_power->setRange(0, 27);
	ui->_sl_power->setTickPosition(QSlider::TicksBelow);
	ui->_sl_power->setTickInterval(4);

	connect(ui->_sl_power, SIGNAL(valueChanged(int)), ui->_sb_power, SLOT(setValue(int)));
	connect(ui->_sb_power, SIGNAL(valueChanged(int)), this, SLOT(_slot_change_value_power(int)));
	connect(ui->_sb_power, SIGNAL(valueChanged(int)), _controller, SLOT(_slot_change_value_power(int)));

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

	connect(ui->_pb_start, SIGNAL(clicked(bool)), this, SLOT(_slot_start_stop_clicked(bool)));
	connect(ui->_pb_start, SIGNAL(clicked(bool)), _controller, SLOT(_slot_start_stop_clicked(bool)));

	ui->_cb_antena->addItem(tr("Направленная"));
	ui->_cb_antena->addItem(tr("Ненапрвленная"));

	connect(ui->_cb_antena, SIGNAL(activated(int)), this, SLOT(_set_antenna_type(int)));
	connect(ui->_cb_antena, SIGNAL(activated(int)), _controller, SLOT(set_antenna_type(int)));

	ui->_cb_mode->addItem(tr("Облучение"));
	ui->_cb_mode->addItem(tr("Приведение"));
	ui->_cb_mode->addItem(tr("Круговой обзор"));
	connect(ui->_cb_mode, SIGNAL(activated(int)), _controller, SLOT(_slot_change_mode(int)));

	ui->_le_lat->setFixedWidth(70);
	ui->_le_lon->setFixedWidth(70);

	ui->_le_status->setReadOnly(true);
	ui->_le_status->setText(tr("Простой"));

	connect(ui->_clear_uvod, SIGNAL(clicked()), this, SLOT(_slot_clear()));
	connect(ui->_clear_uvod, SIGNAL(clicked()), _controller, SLOT(_slot_clear()));
}

NIIPPControlWidget::~NIIPPControlWidget()
{
	delete ui;
}

bool NIIPPControlWidget::getState()
{
	return ui->_pb_enable_complex->isChecked();
}

void NIIPPControlWidget::_slot_enable_complex(bool state)
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

void NIIPPControlWidget::_slot_change_value_power(int value)
{
	ui->_sl_power->setValue(value);

	if(_controller->getAntenaType() == 0) {
		ui->_le_distance->setText(QString("%1 км").arg(QString::number(_controller->getRadiusSector())));
		if(ui->_pb_start->isChecked())
		{
			ui->_le_status->setText(tr("Облучение"));
		}
	}
	if(_controller->getAntenaType() == 1) {
		ui->_le_distance->setText(QString("%1 км").arg(QString::number(_controller->getRadiusCircle())));
		if(ui->_pb_start->isChecked())
		{
			ui->_le_status->setText(tr("Облучение"));
		}
	}
}

void NIIPPControlWidget::_slot_start_stop_clicked(bool state)
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

void NIIPPControlWidget::_set_antenna_type(int value)
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

void NIIPPControlWidget::_slot_clear()
{
	ui->_le_lat->clear();
	ui->_le_lon->clear();
}

void NIIPPControlWidget::set_point(QPointF coord)
{
	QString lat_s = QString::number(coord.x(), 'f', 4);
	ui->_le_lat->setText(lat_s);
	QString lon_s = QString::number(coord.y(), 'f', 4);
	ui->_le_lon->setText(lon_s);
}

bool NIIPPControlWidget::getEnableComplexState()
{
	return ui->_pb_enable_complex->isChecked();
}

bool NIIPPControlWidget::getStartState()
{
	return ui->_pb_start->isChecked();
}

int NIIPPControlWidget::getModeIndex()
{
	return ui->_cb_mode->currentIndex();
}

int NIIPPControlWidget::getAntenaIndex()
{
	return ui->_cb_antena->currentIndex();
}

int NIIPPControlWidget::getSbPowerValue()
{
	return ui->_sb_power->value();
}

void NIIPPControlWidget::setLeStatusText(int mode)
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

void NIIPPControlWidget::setLatText(QString text)
{
	ui->_le_lat->setText(text);
}

void NIIPPControlWidget::setLonText(QString text)
{
	ui->_le_lon->setText(text);
}
