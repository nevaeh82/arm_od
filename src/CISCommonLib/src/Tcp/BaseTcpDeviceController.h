#ifndef BASETCPDEVICECONTROLLER_H
#define BASETCPDEVICECONTROLLER_H

#include <QObject>
#include <QVariant>

#include "Templates/BaseSubject.h"
#include "Interfaces/Tcp/ITcpReceiver.h"
#include "Interfaces/Tcp/ITcpListener.h"
#include "Interfaces/Tcp/ITcpDeviceController.h"

#include "BaseTcpClient.h"
#include "BaseTcpDeviceCoder.h"

#include "Logger/Logger.h"

#define BASE_TCP_DEVICE "baseTcpDevice"

class BaseTcpDeviceController : public QObject, public ITcpDeviceController, public ITcpReceiver, public BaseSubject<ITcpListener>
{
	Q_OBJECT

protected:
	BaseTcpClient* m_tcpClient;
	BaseTcpDeviceCoder* m_tcpDeviceCoder;
	QString m_tcpDeviceName;
	quint32 m_deviceType;

	QString m_host;
	quint32 m_port;

	QByteArray m_deviceBaseInfo;
	QByteArray m_deviceFullInfo;

public:
	explicit BaseTcpDeviceController(const QString& tcpDeviceName = BASE_TCP_DEVICE, QObject* parent = NULL);
	virtual ~BaseTcpDeviceController();

	virtual void createTcpClient();
	virtual void createTcpDeviceCoder();

	virtual void connectToHost(const QString& host, const quint32& port);
	virtual void connectToHost();
	virtual void disconnectFromHost();
	virtual bool isConnected();
	virtual QString getHost();

	virtual void sendData(const MessageSP message);
	virtual QObject* asQObject();

	virtual bool init();
	virtual void setDeviceInfo(const QByteArray& baseInfo,
							   const QByteArray& fullInfo);
	virtual QByteArray getFullInfo();

	virtual void onDataReceived(const QVariant& argument); // from BaseTcpClient

	virtual void setReconnectInterval(const uint timeMsec);

signals:

	void signalTcpDeviceConnectedToHost(int status);
};

#endif // BASETCPDEVICECONTROLLER_H
