#include "Solver.h"

Solver::Solver(int id, ITabManager *tabManager)
{
	m_id = id;
	m_tabManager = tabManager;

	m_autoStateValue = true;
	m_height = 0;
	m_trackLength = 0;
}

Solver::~Solver()
{
}

void Solver::setAutoState(bool val)
{
	m_autoStateValue = val;
}

void Solver::setHeight(double val)
{
	m_height = val;
}

void Solver::setTrackLength(int val)
{
	m_trackLength = val;
}

void Solver::accept()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	ds << m_id;
	ds << m_trackLength;
	ds << m_height;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER, ba);
	m_tabManager->send_data(0, msg);
}

void Solver::autoState()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	ds << m_autoStateValue;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_AUTO, ba);
	m_tabManager->send_data(0, msg);
}

void Solver::clear()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	bool t =true;
	ds << m_id;
	ds << t;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_CLEAR, ba);
	m_tabManager->send_data(0, msg);
}

void Solver::countTrack(int count)
{
}

