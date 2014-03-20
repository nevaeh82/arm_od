#include "Solver.h"

Solver::Solver(int id, ITabManager *tab_manager) :
	_widget(new SolverWidget())
{
	_id = id;
	_tab_manager = tab_manager;

	connect(_widget, SIGNAL(signalClear()), this, SLOT(slotClear()));
	connect(_widget, SIGNAL(signalAccept()), this, SLOT(slotAccept()));
	connect(_widget, SIGNAL(signalCancel()), this, SLOT(slotCancel()));
	connect(_widget, SIGNAL(signalAuto(bool)), this, SLOT(slotAuto(bool)));
}

Solver::~Solver()
{
	delete _widget;
}

QWidget*Solver::getWidget()
{
	return _widget;
}

void Solver::slotAccept()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	ds << _id;
	ds << _widget->getTrackLength();
	ds << _widget->getHeight();

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER, ba);
	_tab_manager->send_data(0, msg);
}

void Solver::slotCancel()
{
	_widget->hide();
}

void Solver::slotAuto(bool state)
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	ds << _widget->getAutoState();

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_AUTO, ba);
	_tab_manager->send_data(0, msg);
}

void Solver::slotClear()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	bool t =true;
	ds << _id;
	ds << t;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_CLEAR, ba);
	_tab_manager->send_data(0, msg);
}

void Solver::slotCountTrack(int count)
{
}

void Solver::slotShow()
{
	_widget->show();
}
