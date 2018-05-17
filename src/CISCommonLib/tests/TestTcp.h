#ifndef TESTTCP_H
#define TESTTCP_H

#include <cxxtest/TestSuite.h>

#include <QThread>
#include <QTcpServer>
#include <QTime>

#include <QxtSignalWaiter>

#include "QApplicationRunner.h"

#include "Tcp/BaseTcpDeviceController.h"



class Listener : public QObject
{
	Q_OBJECT
private:
	int m_status;

	QTcpServer* m_tcpServer;

public:
	Listener()
	{
		m_status = -1;

		m_tcpServer = new QTcpServer(this);
	}
	~Listener() {
		m_tcpServer->close();
	}

	int getStatus() const
	{
		return m_status;
	}

	bool start() {
		return m_tcpServer->listen(QHostAddress::Any, 5995);
	}

	void stop()
	{
		m_tcpServer->close();
	}

	void sendDeleteSignal() {
		emit threadTerminateSignal();
	}

signals:
	void threadTerminateSignal();

public slots:

	void onConnectionStatusChanged(int status) {
		m_status = status;
	}
};

class TestTcp: public CxxTest::TestSuite
{

public:

	void testConnectAndSendConnected()
	{
		Listener* lstn = new Listener;

		bool res = lstn->start();
		TS_ASSERT_EQUALS(true, res);

		QApplicationRunner runner;

		BaseTcpDeviceController* controller = new BaseTcpDeviceController;

		controller->createTcpClient();
		controller->createTcpDeviceCoder();

		QThread* controllerThread = new QThread;
		QObject::connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
		controller->asQObject()->moveToThread(controllerThread);
		controllerThread->start();

		QObject::connect(controller, SIGNAL(signalTcpDeviceConnectedToHost(int)), lstn, SLOT(onConnectionStatusChanged(int)));

		controller->connectToHost("127.0.0.1", 5995);

		runner.exec(1500);

		TS_ASSERT_EQUALS( 1, lstn->getStatus() );

		lstn->sendDeleteSignal();
		runner.exec(1000);
		delete lstn;
	}


	void testConnectAndSendFailed()
	{
		Listener* lstn = new Listener;

		QApplicationRunner runner;

		BaseTcpDeviceController* controller = new BaseTcpDeviceController;

		controller->createTcpClient();
		controller->createTcpDeviceCoder();

		QThread* controllerThread = new QThread;
		QObject::connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
		controller->asQObject()->moveToThread(controllerThread);
		controllerThread->start();

		QObject::connect(controller, SIGNAL(signalTcpDeviceConnectedToHost(int)), lstn, SLOT(onConnectionStatusChanged(int)));

		controller->connectToHost("127.0.0.1", 5997);

		runner.exec(1500);

		TS_ASSERT_EQUALS( 0, lstn->getStatus() );

		lstn->sendDeleteSignal();
		runner.exec(1000);
		delete lstn;
	}


	void testConnectAndReconnect()
	{
		Listener* lstn = new Listener;
		bool res = lstn->start();
		TS_ASSERT_EQUALS(true, res);

		QApplicationRunner runner;

		BaseTcpDeviceController* controller = new BaseTcpDeviceController;

		controller->createTcpClient();
		controller->createTcpDeviceCoder();

		QThread* controllerThread = new QThread;
		QObject::connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
		controller->asQObject()->moveToThread(controllerThread);
		controllerThread->start();

		QObject::connect(controller, SIGNAL(signalTcpDeviceConnectedToHost(int)), lstn, SLOT(onConnectionStatusChanged(int)));
		QObject::connect(controller, SIGNAL(signalTcpDeviceConnectedToHost(int)), &runner, SLOT(onEvent()));

		controller->connectToHost("127.0.0.1", 5995);

		runner.exec(1500);

		TS_ASSERT_EQUALS( 1, lstn->getStatus() );

		lstn->stop();

		runner.exec(3000);

		TS_ASSERT_EQUALS( 0, lstn->getStatus() );
		
		res = lstn->start();
		TS_ASSERT_EQUALS(true, res);

		runner.exec(3500);

		TS_ASSERT_EQUALS( 1, lstn->getStatus() );

		lstn->sendDeleteSignal();
		runner.exec(1000);
		delete lstn;
	}

	void testConnectFailedAndReconnect()
	{
		Listener* lstn = new Listener;

		QApplicationRunner runner;

		BaseTcpDeviceController* controller = new BaseTcpDeviceController;

		controller->createTcpClient();
		controller->createTcpDeviceCoder();

		QThread* controllerThread = new QThread;
		QObject::connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
		controller->asQObject()->moveToThread(controllerThread);
		controllerThread->start();

		QObject::connect(controller, SIGNAL(signalTcpDeviceConnectedToHost(int)), lstn, SLOT(onConnectionStatusChanged(int)));

		// BECAUSE VASILIEV TEST SYSTEM IS BAD
		//QObject::connect(controller, SIGNAL(signalTcpDeviceConnectedToHost(int)), &runner, SLOT(onEvent()));



		controller->connectToHost("127.0.0.1", 5995);

		runner.exec(4000);

		TS_ASSERT_EQUALS( 0, lstn->getStatus() );

		bool res = lstn->start();
		TS_ASSERT_EQUALS(true, res);

		//runner.exec(3500);
		QxtSignalWaiter::wait(controller, SIGNAL(signalTcpDeviceConnectedToHost(int)), 8000);

		TS_ASSERT_EQUALS( 1, lstn->getStatus() );

		lstn->sendDeleteSignal();
		runner.exec(1000);
		delete lstn;
	}

	void testChangeReconnectInterval()
	{
		Listener* lstn = new Listener;
		lstn->start();

		QApplicationRunner runner;

		BaseTcpDeviceController* controller = new BaseTcpDeviceController;

		controller->createTcpClient();
		controller->createTcpDeviceCoder();

		controller->setReconnectInterval(3000);

		QThread* controllerThread = new QThread;
		QObject::connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
		QObject::connect(lstn, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
		controller->asQObject()->moveToThread(controllerThread);
		controllerThread->start();

		QObject::connect(controller, SIGNAL(signalTcpDeviceConnectedToHost(int)), lstn, SLOT(onConnectionStatusChanged(int)));
		QObject::connect(controller, SIGNAL(signalTcpDeviceConnectedToHost(int)), &runner, SLOT(onEvent()));

		controller->connectToHost("127.0.0.1", 5995);

		runner.exec(1500);

		TS_ASSERT_EQUALS( 1, lstn->getStatus() );

		lstn->stop();

		runner.exec(3500);

		TS_ASSERT_EQUALS( 0, lstn->getStatus() );

		bool res = lstn->start();
		TS_ASSERT_EQUALS(true, res);

		runner.exec(100);

		TS_ASSERT_EQUALS( 0, lstn->getStatus() );

		runner.exec(3500);

		TS_ASSERT_EQUALS( 1, lstn->getStatus() );

		lstn->sendDeleteSignal();
		runner.exec(1000);
		delete lstn;
	}


};

#endif // TESTTCP_H
