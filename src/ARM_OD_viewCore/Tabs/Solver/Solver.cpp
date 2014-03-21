#include "Solver.h"

Solver::Solver(int id, ITabManager *tab_manager)
{
	_id = id;
	_tab_manager = tab_manager;

	_autoStateValue = true;
	_height = 0;
	_trackLength = 0;
}

Solver::~Solver()
{
}

void Solver::setAutoState(bool val)
{
	_autoStateValue = val;
}

void Solver::setHeight(double val)
{
	_height = val;
}

void Solver::setTrackLength(int val)
{
	_trackLength = val;
}

void Solver::accept()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	ds << _id;
	ds << _trackLength;
	ds << _height;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER, ba);
	_tab_manager->send_data(0, msg);
}

void Solver::autoState()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	ds << _autoStateValue;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_AUTO, ba);
	_tab_manager->send_data(0, msg);
}

void Solver::clear()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	bool t =true;
	ds << _id;
	ds << t;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_CLEAR, ba);
	_tab_manager->send_data(0, msg);
}

void Solver::countTrack(int count)
{
}

