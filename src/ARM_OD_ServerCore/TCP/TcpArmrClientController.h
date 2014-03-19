#ifndef TCPARMRCLIENTCONTROLLER_H
#define TCPARMRCLIENTCONTROLLER_H

#include <QObject>
#include "Tcp/BaseTcpDeviceController.h"
#include "TcpArmrClientCoder.h"

#include "Logger.h"

#include "Tcp/Defines.h"

class TcpArmrClientController : public BaseTcpDeviceController
{
	Q_OBJECT
public:
	explicit TcpArmrClientController(QObject *parent = 0);
	explicit TcpArmrClientController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpArmrClientController();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();
	virtual QObject* asQObject();

signals:
	void createTcpArmrCoderInternalSignal();

private slots:
	void createTcpArmrCoderInternalSlot();
};

#endif // TCPARMRCLIENTCONTROLLER_H
