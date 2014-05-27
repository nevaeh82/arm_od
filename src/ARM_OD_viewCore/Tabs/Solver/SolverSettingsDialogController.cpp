#include "SolverSettingsDialogController.h"

SolverSettingsDialogController::SolverSettingsDialogController(int id, ITabManager* tabManager, QObject* parent) :
	QObject(parent)
{
	m_id = id;
	m_tabManager = tabManager;

	m_autoStateValue = false;
	m_height = 0;
	m_trackLength = 0;
	m_countNum = 0;
}

SolverSettingsDialogController::~SolverSettingsDialogController()
{
}

void SolverSettingsDialogController::appendView(SolverSettingsDialog *view)
{
	m_view = view;

	m_view->setHeight(m_height);
	m_view->setTrackLength(m_trackLength);
	m_view->setCountNum(m_countNum);
	m_view->setAutoState(m_autoStateValue);

	connect( m_view, SIGNAL(signalClear()), this, SLOT(clear()) );
	connect( m_view, SIGNAL(signalAccept()), this, SLOT(accept()) );
	connect( m_view, SIGNAL(signalAuto(bool)), this, SLOT(autoState(bool)) );
	connect( m_view, SIGNAL(signalCancel()), this, SLOT(cancel()) );
}

void SolverSettingsDialogController::cancel()
{
	m_view->done(SOLVERSETTINGDIALOGCANCEL);
}

void SolverSettingsDialogController::accept()
{
	setTrackLength(m_view->getTrackLength());
	setHeight(m_view->getHeight());
	setCountNum(m_view->getCountNum());

	m_view->done(SOLVERSETTINGDIALOGACCEPT);
}

void SolverSettingsDialogController::setAutoState(bool val)
{
	m_autoStateValue = val;
}

void SolverSettingsDialogController::setHeight(double val)
{
	m_height = val;
}

void SolverSettingsDialogController::setTrackLength(int val)
{
	m_trackLength = val;
}

void SolverSettingsDialogController::setCountNum(int val)
{
	m_countNum = val;
}

void SolverSettingsDialogController::autoState(bool)
{
	setAutoState(m_view->getAutoState());

	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	ds << m_autoStateValue;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_AUTO, ba);
	m_tabManager->send_data(0, msg);
}

void SolverSettingsDialogController::clear()
{
	m_autoStateValue = false;
	m_height = 0;
	m_trackLength = 0;
	m_countNum = 0;

	m_view->setHeight(m_height);
	m_view->setTrackLength(m_trackLength);
	m_view->setCountNum(m_countNum);
	m_view->setAutoState(m_autoStateValue);
	//m_view->done(SOLVERSETTINGDIALOGCLEAR);
}
