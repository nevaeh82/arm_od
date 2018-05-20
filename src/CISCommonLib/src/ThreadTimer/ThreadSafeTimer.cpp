#include "ThreadSafeTimer.h"

ThreadSafeTimer::ThreadSafeTimer(QObject* parent) :
	QTimer(parent)
{
	connect(this, SIGNAL(startSignal(int)), this, SLOT(startSlot(int)));
	connect(this, SIGNAL(startSignal()), this, SLOT(startSlot()));
	connect(this, SIGNAL(stopSignal()), this, SLOT(stopSlot()));
}

ThreadSafeTimer::~ThreadSafeTimer()
{
}

void ThreadSafeTimer::start(int msec)
{
	emit startSignal(msec);
}

void ThreadSafeTimer::start()
{
	emit startSignal();
}

void ThreadSafeTimer::stop()
{
	emit stopSignal();
}

void ThreadSafeTimer::startSlot(int msec)
{
	QTimer::start(msec);
}

void ThreadSafeTimer::startSlot()
{
	QTimer::start();
}

void ThreadSafeTimer::stopSlot()
{
	QTimer::stop();
}
