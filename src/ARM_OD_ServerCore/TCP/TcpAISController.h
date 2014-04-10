#ifndef TCPAISCONTROLLER_H
#define TCPAISCONTROLLER_H

#include <Tcp/BaseTcpDeviceController.h>

#include <QSettings>
#include <QDataStream>

#include "TcpDevicesDefines.h"
#include "TcpAISCoder.h"
#include "TcpAISClient.h"
#include "TcpAISZoneManager.h"

#include "Info/BaseSettings.h"
#include "Info/AISSettings.h"

#define TCP_AIS_TIMER_LIFE	20000

class TcpAISController : public BaseTcpDeviceController
{
	Q_OBJECT

private:
	AISSettings m_AISSettingStruct;
	ITcpAISZoneManager* m_zoneManager;
	QTimer m_updateTimer;

public:
	explicit TcpAISController(const QString& tcpDeviceName = BASE_TCP_DEVICE, QObject* parent = NULL);
	virtual ~TcpAISController();

	// ITcpDeviceController interface
public:
	virtual void createTcpClient();
	virtual void createTcpDeviceCoder();
	virtual void connectToHost(const QString& host, const quint32& port);
	virtual void disconnectFromHost();
	virtual QObject* asQObject();

	virtual void connectToHost();
	virtual bool init();
	virtual QByteArray getFullInfo();

signals:
	void connectToHostAISInternalSignal(const QString& host, const quint32& port);
	void createTcpAISCoderInternalSignal();
	void createTcpAISClientInternalSignal();

private slots:
	void connectToHostAISInternalSlot(const QString& host, const quint32& port);
	void createTcpAISCoderInternalSlot();
	void createTcpAISClientInternalSlot();
	void updateDataInternalSlot();
};

#endif // TCPAISCONTROLLER_H
