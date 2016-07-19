#pragma once

#include "Tcp/BaseADSBController.h"
#include <Tcp/BaseTcpDeviceController.h>

#include <QSettings>
#include <QDataStream>

#include "TcpDevicesDefines.h"
#include "TCP/TcpDefines.h"

#include "Info/BaseSettings.h"
#include "Info/AISSettings.h"

#include "PlanesClient.h"

class TcpADSBController : public BaseADSBController
{
	Q_OBJECT

public:
	explicit TcpADSBController(const QString& tcpDeviceName = BASE_TCP_DEVICE, QObject* parent = NULL);
	virtual ~TcpADSBController();

	void enableAdsb(bool);

private:
	PlanesClient* m_client;

	//settings
	QString m_processPath;
	QString m_processName;

	QString m_adsbHost;
	uint m_adsbPort;
	uint m_riPort;
	uint m_sbsPort;

private slots:
	void onPlanesDataReceived(QByteArray);

	void onStart();
	void onStop();

	void onInitPlanesClient();

signals:
	void signalStart();
	void signalStop();

};

