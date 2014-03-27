#include "SolverController.h"

SolverController::SolverController(int id, ITabManager* tabManager)
	: m_widget( new SolverWidget() )
	, m_model( new Solver(id, tabManager) )
{
	connect( m_widget, SIGNAL(signalClear()), this, SLOT(clear()) );
	connect( m_widget, SIGNAL(signalAccept()), this, SLOT(accept()) );
	connect( m_widget, SIGNAL(signalAuto(bool)), this, SLOT(autoState(bool)) );
	connect( m_widget, SIGNAL(signalCancel()), this, SLOT(cancel()) );
}

SolverController::~SolverController()
{
	delete m_widget;
	delete m_model;
}

void SolverController::show()
{
	m_widget->show();
}

QWidget* SolverController::getWidget()
{
	return m_widget;
}

Solver* SolverController::getModel()
{
	return m_model;
}

void SolverController::cancel()
{
	m_widget->hide();
}

void SolverController::accept()
{
	m_model->setTrackLength(m_widget->getTrackLength());
	m_model->setHeight(m_widget->getHeight());

	m_model->accept();
}

void SolverController::autoState(bool)
{
	m_model->setAutoState(m_widget->getAutoState());

	m_model->autoState();
}

void SolverController::clear()
{
	m_model->clear();
}
