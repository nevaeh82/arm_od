#ifndef TCPAISCONTROLLER_H
#define TCPAISCONTROLLER_H

#include <Tcp/BaseTcpDeviceController.h>

#include "TcpDevicesDefines.h"
#include "TcpAISCoder.h"
#include "TcpAISClient.h"
#include "TcpAISZoneManager.h"

class TcpAISController : public BaseTcpDeviceController
{
	Q_OBJECT

private:
	ITcpAISZoneManager* m_zoneManager;

public:
	explicit TcpAISController(QObject* parent = NULL);
	explicit TcpAISController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpAISController();

	// ITcpDeviceController interface
public:
	virtual void createTcpClient();
	virtual void createTcpDeviceCoder();
	virtual void connectToHost(const QString& host, const quint32& port);
	virtual void disconnectFromHost();
	virtual QObject* asQObject();

signals:
	void createTcpAISCoderInternalSignal();
	void createTcpAISClientInternalSignal();

private slots:
	void createTcpAISCoderInternalSlot();
	void createTcpAISClientInternalSlot();
};

#endif // TCPAISCONTROLLER_H
