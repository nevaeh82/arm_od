#include "ui_SolverWidget.h"

#include "SolverWidget.h"


SolverWidget::SolverWidget(QWidget *parent)
	: QWidget(parent)
	, m_ui(new Ui::SolverWidget)
{
	m_ui->setupUi(this);

	setWindowIconText(tr("Detector"));

	m_ui->_chb_auto->setCheckState(Qt::Unchecked);

	m_ui->_dsb_alt->setValue(0);
	m_ui->_dsb_alt->setRange(0, 10000);

	m_ui->_sb_track_length->setRange(0, 1000);
	m_ui->_sb_track_length->setValue(0);

	m_ui->_sb_count_point_for_state->setRange(0, 1000);
	m_ui->_sb_count_point_for_state->setValue(0);

	connect(m_ui->_pb_clear, SIGNAL(clicked()), this, SIGNAL(signalClear()));
	connect(m_ui->_accept, SIGNAL(clicked()), this, SIGNAL(signalAccept()));
	connect(m_ui->_cancel, SIGNAL(clicked()), this, SIGNAL(signalCancel()));

	connect(m_ui->_chb_auto, SIGNAL(toggled(bool)), this, SIGNAL(signalAuto(bool)));
}

int SolverWidget::getTrackLength()
{
	return m_ui->_sb_track_length->value();
}

double SolverWidget::getHeight()
{
	return m_ui->_dsb_alt->value();
}

int SolverWidget::getCountNum()
{
	return m_ui->_sb_count_point_for_state->value();
}

bool SolverWidget::getAutoState()
{
	return m_ui->_chb_auto->isChecked();
}
