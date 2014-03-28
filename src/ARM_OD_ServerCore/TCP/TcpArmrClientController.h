#ifndef TCPARMRCLIENTCONTROLLER_H
#define TCPARMRCLIENTCONTROLLER_H

#include <QObject>
#include <QSettings>
#include "Tcp/BaseTcpDeviceController.h"
#include "TcpArmrClientCoder.h"
#include "Info/BaseSettings.h"
#include "Info/ArmrTcpClientSettings.h"

#include "Logger.h"

#include "TcpDevicesDefines.h"

class TcpArmrClientController : public BaseTcpDeviceController
{
	Q_OBJECT
private:
	ArmrTcpClientSettings m_settings;

public:
	explicit TcpArmrClientController(QObject *parent = 0);
	explicit TcpArmrClientController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpArmrClientController();

	virtual bool init();
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
