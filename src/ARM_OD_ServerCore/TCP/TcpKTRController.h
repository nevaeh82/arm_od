#ifndef TCPKTRCONTROLLER_H
#define TCPKTRCONTROLLER_H

#include <Tcp/BaseTcpDeviceController.h>

#include <QSettings>
#include <QDataStream>

#include "TcpDevicesDefines.h"
#include "TcpKTRCoder.h"
#include "Info/BaseSettings.h"
#include "Info/KTRSettings.h"

class TcpKTRController : public BaseTcpDeviceController
{
	Q_OBJECT

private:
	KTRSettings m_KTRSettingStruct;

public:
	explicit TcpKTRController(const QString& tcpDeviceName = BASE_TCP_DEVICE, QObject* parent = NULL);
	virtual ~TcpKTRController();

	// ITcpDeviceController interface
public:
	virtual void createTcpClient();
	virtual void createTcpDeviceCoder();
	virtual bool init();
	virtual QByteArray getFullInfo();

signals:
	void createTcpKTRCoderInternalSignal();

private slots:
	void createTcpKTRCoderInternalSlot();
	void tcpConnectionStatusInternalSlot(int status);
};

#endif // TCPKTRCONTROLLER_H
