#include "ui_SolverWidget.h"

#include "SolverWidget.h"


SolverWidget::SolverWidget(QWidget *parent) :
	QWidget(parent)
	, _ui(new Ui::SolverWidget)
{
	_ui->setupUi(this);

	setWindowIconText(tr("Обнаружитель"));

	_ui->_chb_auto->setCheckState(Qt::Unchecked);

	_ui->_dsb_alt->setValue(0);
	_ui->_dsb_alt->setRange(0, 10000);

	_ui->_sb_track_length->setRange(0, 1000);
	_ui->_sb_track_length->setValue(0);

	_ui->_sb_count_point_for_state->setRange(0, 1000);
	_ui->_sb_count_point_for_state->setValue(0);

	connect(_ui->_pb_clear, SIGNAL(clicked()), this, SIGNAL(signalClear()));
	connect(_ui->_accept, SIGNAL(clicked()), this, SIGNAL(signalAccept()));
	connect(_ui->_cancel, SIGNAL(clicked()), this, SIGNAL(signalCancel()));

	connect(_ui->_chb_auto, SIGNAL(toggled(bool)), this, SIGNAL(signalAuto(bool)));
}

int SolverWidget::getTrackLength()
{
	return _ui->_sb_track_length->value();
}

double SolverWidget::getHeight()
{
	return _ui->_dsb_alt->value();
}

int SolverWidget::getCountNum()
{
	return _ui->_sb_count_point_for_state->value();
}

bool SolverWidget::getAutoState()
{
	return _ui->_chb_auto->isChecked();
}
