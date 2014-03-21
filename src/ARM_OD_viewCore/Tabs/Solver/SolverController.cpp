#include "SolverController.h"

SolverController::SolverController(int id, ITabManager* tab_manager) :
	_widget(new SolverWidget())
	, _model(new Solver(id, tab_manager))
{
	connect(_widget, SIGNAL(signalClear()), _model, SLOT(clear()));
	connect(_widget, SIGNAL(signalAccept()), this, SLOT(accept()));
	connect(_widget, SIGNAL(signalAuto(bool)), this, SLOT(autoState(bool)));

	connect(_widget, SIGNAL(signalCancel()), this, SLOT(cancel()));
}

SolverController::~SolverController()
{
	_widget->deleteLater();
	_model->deleteLater();
}

void SolverController::show()
{
	_widget->show();
}

QWidget* SolverController::getWidget()
{
	return _widget;
}

Solver* SolverController::getModel()
{
	return _model;
}

void SolverController::cancel()
{
	_widget->hide();
}

void SolverController::accept()
{
	_model->setTrackLength(_widget->getTrackLength());
	_model->setHeight(_widget->getHeight());

	_model->accept();
}

void SolverController::autoState(bool)
{
	_model->setAutoState(_widget->getAutoState());

	_model->autoState();
}
