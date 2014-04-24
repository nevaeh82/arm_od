#ifndef TCPNIIPPCONTROLLER_H
#define TCPNIIPPCONTROLLER_H

#include <Tcp/BaseTcpDeviceController.h>

#include <QSettings>
#include <QDataStream>

#include "TcpDevicesDefines.h"
#include "TcpNIIPPCoder.h"
#include "Info/BaseSettings.h"
#include "Info/NIIPPSettings.h"

class TcpNIIPPController : public BaseTcpDeviceController
{
	Q_OBJECT

private:
	NIIPPSettings m_NIIPPSettingStruct;
    int m_id;

public:
	explicit TcpNIIPPController(const QString& tcpDeviceName = BASE_TCP_DEVICE, QObject* parent = NULL);
	virtual ~TcpNIIPPController();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();
	virtual bool init();
	virtual QByteArray getFullInfo();

signals:
	void createTcpNIIPPCoderInternalSignal();

private slots:
	void createTcpNIIPPCoderInternalSlot();
};

#endif // TCPNIIPPCONTROLLER_H
