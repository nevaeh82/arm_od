#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QThread>
#include <QMap>

#include <Interfaces/Tcp/ITcpListener.h>
#include <Interfaces/IRpcListener.h>
#include <Logger.h>

#include "Common/Defines.h"

#include "Interfaces/Tcp/ITcpManager.h"
#include "Interfaces/Tcp/ITcpServer.h"

#include "TcpDevicesDefines.h"
#include "TcpNIIPPController.h"
#include "TcpKTRController.h"

#include "TcpArmrClientController.h"

#include "Rpc/RPCServer.h"
#include "Rpc/RpcDefines.h"

class TcpManager : public QObject, public ITcpManager, public ITcpListener, public IRpcListener
{
	Q_OBJECT

private:

	/// TODO
	IRPC* m_rpcServer;
	ITcpServer* m_tcpServer;

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
	virtual void addTcpDevice(const QString& deviceType, const QString& host, const quint32& port);
	virtual void removeTcpDevice(const QString& deviceType, const QString& host, const quint32& port);
	virtual void setRpcServer(IRPC* rpcServer);
	virtual void setTcpServer(ITcpServer* tcpServer);
	virtual QObject* asQObject();

	// ITcpListener interface
public:
	virtual void onMessageReceived(const QString& device, const MessageSP argument);

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

signals:
	void threadTerminateSignal();
	void onMethodCalledInternalSignal(const QString& method, const QVariant& argument);

private slots:
	void onMethodCalledInternalSlot(const QString &method, const QVariant &argument);

};

#endif // TCPMANAGER_H
