#ifndef THREADSAFETIMER_H
#define THREADSAFETIMER_H

#include <QTimer>

class ThreadSafeTimer : public QTimer
{
	Q_OBJECT

public:
	explicit ThreadSafeTimer(QObject* parent = NULL);
	virtual ~ThreadSafeTimer();

	virtual void start(int msec);

	virtual void start();
	virtual void stop();

signals:
	void startSignal(int);
	void startSignal();

	void stopSignal();

private slots:
	void startSlot(int msec);

	void startSlot();
	void stopSlot();
	
};

#endif // THREADSAFETIMER_H
