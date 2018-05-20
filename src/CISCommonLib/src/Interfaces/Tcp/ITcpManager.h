#ifndef ITCPMANAGER_H
#define ITCPMANAGER_H

#include <qglobal.h>

class IRPC;
class QObject;
class ITcpServer;

class ITcpManager
{

public:
	virtual ~ITcpManager() {}

	virtual void addTcpDevice(const QString& deviceName, const quint32& deviceType)	= 0;
	virtual void removeTcpDevice(const QString& deviceName)	= 0;

	virtual void setRpcServer(IRPC* rpcServer)			= 0;
	virtual void setTcpServer(ITcpServer* tcpServer)	= 0;

	virtual QObject* asQObject()	= 0;
};

#endif // ITCPMANAGER_H
