#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QThread>
#include <QMap>

#include <Interfaces/IRpcListener.h>
#include <Interfaces/Tcp/ITcpListener.h>
#include <Interfaces/Tcp/ITcpManager.h>
#include <Logger.h>

#include "TcpDevicesDefines.h"
#include "TcpNIIPPController.h"
#include "TcpKTRController.h"
#include "TcpAISController.h"

#include "Rpc/RPCServer.h"

class TcpManager : public QObject, public ITcpManager, public ITcpListener, public IRpcListener
{
	Q_OBJECT

private:

	/// TODO
	IRPC* m_rpcServer;

	QMap< QString, BaseTcpDeviceController* > m_controllersMap;
	/**
	 * key = deviceType
	 * Define own deviceType in TcpDevicesDefines
	 *
	 **/


public:
	explicit TcpManager(QObject* parent = NULL);
	virtual ~TcpManager();

	// ITcpManager interface
public:
	virtual void addTcpDevice(const QString& deviceName, const quint32& deviceType);
	virtual void removeTcpDevice(const QString& deviceName);
	virtual void setRpcServer(IRPC* rpcServer);
	virtual QObject* asQObject();
	virtual void setTcpServer(ITcpServer* tcpServer);

	// ITcpListener interface
public:
	virtual void onMessageReceived(const quint32 deviceType, const QString& deviceName, const MessageSP argument);

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

signals:
	void threadTerminateSignal();
	void onMethodCalledInternalSignal(const QString& method, const QVariant& argument);

private slots:
	void onMethodCalledInternalSlot(const QString &method, const QVariant &argument);


	// ITcpManager interface
};

#endif // TCPMANAGER_H
