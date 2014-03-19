#ifndef ITCPMANAGER_H
#define ITCPMANAGER_H

#include <qglobal.h>

class IRPC;
class QObject;

class ITcpManager
{

public:
	virtual ~ITcpManager() {}

	virtual void addTcpDevice(const QString& deviceType, const QString& host, const quint32& port)	= 0;
	virtual void removeTcpDevice(const QString& deviceType, const QString& host, const quint32& port)	= 0;
	virtual void setRpcServer(IRPC* rpcServer)	= 0;
	virtual QObject* asQObject()	= 0;

	/// TODO: add sendDataToDevice from some internal message
};

#endif // ITCPMANAGER_H
