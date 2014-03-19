#ifndef TCPNIIPPCONTROLLER_H
#define TCPNIIPPCONTROLLER_H

#include <Tcp/BaseTcpDeviceController.h>

#include "TcpDevicesDefines.h"
#include "TcpNIIPPCoder.h"

class TcpNIIPPController : public BaseTcpDeviceController
{
	Q_OBJECT

public:
	explicit TcpNIIPPController(QObject* parent = NULL);
	explicit TcpNIIPPController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpNIIPPController();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();

signals:
	void createTcpNIIPPCoderInternalSignal();

private slots:
	void createTcpNIIPPCoderInternalSlot();
};

#endif // TCPNIIPPCONTROLLER_H
