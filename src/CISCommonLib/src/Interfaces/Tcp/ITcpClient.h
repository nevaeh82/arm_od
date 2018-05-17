#pragma once

#include <qglobal.h>
class QObject;
class QIODevice;

class ITcpClient
{

public:
	virtual ~ITcpClient() {}

	virtual void connectToHostAsync(const QString& host, const quint32 port)	= 0;
	virtual void disconnectFromHostAsync() = 0;

	virtual bool connectToHostSync(const QString& host, const quint32 port)	= 0;
	virtual bool disconnectFromHostSync() = 0;

	virtual bool isConnected() = 0;
	virtual QString getHost() = 0; // Concatenation of host, ":" and port
	virtual QObject* asQObject() = 0;
	virtual void setReconnectInterval(const uint timeMsec) = 0;
};
