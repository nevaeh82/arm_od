#ifndef BASETCPDEVICECONTROLLER_H
#define BASETCPDEVICECONTROLLER_H

#include <QObject>
#include <QVariant>

#include <PwLogger/PwLogger.h>

#include "BaseSubject.h"
#include "../Interfaces/ITcpReceiver.h"
#include "../Interfaces/ITcpListener.h"
#include "../Interfaces/ITcpDeviceController.h"
#include "TcpDevicesDefines.h"

#include "BaseTcpClient.h"
#include "BaseTcpDeviceCoder.h"

class BaseTcpDeviceController : public QObject, public ITcpDeviceController, public ITcpReceiver, public BaseSubject<ITcpListener>
{
	Q_OBJECT

protected:
	BaseTcpClient* m_tcpClient;
	BaseTcpDeviceCoder* m_tcpDeviceCoder;
	QString m_tcpDeviceName;

	Pw::Logger::ILogger* m_logger;

public:
	explicit BaseTcpDeviceController(Pw::Logger::ILogger* logger, QObject* parent = NULL);
	explicit BaseTcpDeviceController(const QString& tcpDeviceName, Pw::Logger::ILogger* logger, QObject* parent = NULL);
	virtual ~BaseTcpDeviceController();

	// ITcpDeviceController interface
public:
	virtual void createTcpClient();
	virtual void createTcpDeviceCoder();
	virtual void connectToHost(const QString& host, const quint32& port);
	virtual void disconnectFromHost();
	virtual bool isConnected();
	virtual QString getHost();
	virtual void sendData(const MessageSP message);
	virtual QObject* asQObject();

	// ITcpReceiver interface
public:
	virtual void onDataReceived(const QVariant& argument); // from BaseTcpClient

signals:
	void connectToHostInternalSignal(const QString& host, const quint32& port);
	void disconnectFromHostInternalSignal();
	void sendDataInternalSignal(const MessageSP message);
	void onDataReceivedInternalSignal(const QVariant& argument);
	void createTcpClientInternalSignal();
	void createTcpDeviceCoderInternalSignal();

private slots:
	void connectToHostInternalSlot(const QString& host, const quint32& port);
	void disconnectFromHostInternalSlot();
	void sendDataInternalSlot(const MessageSP message);
	void onDataReceivedInternalSlot(const QVariant& argument);
	void createTcpClientInternalSlot();
	void createTcpDeviceCoderInternalSlot();

};

#endif // BASETCPDEVICECONTROLLER_H
