#pragma once

#include <qglobal.h>
class QObject;
class QIODevice;

class ITcpServerClient
{

public:
	virtual ~ITcpServerClient() {}

	virtual void connectToHost(const QString& host, const quint32 port)	= 0;
	virtual void disconnectFromHost() = 0;
	virtual bool isConnected() = 0;

	virtual void writeData(const QByteArray& data)	= 0;

	virtual QString getHost() = 0; // Concatenation of host, ":" and port
	virtual QObject* asQObject() = 0;
	virtual void setReconnectInterval(const uint timeMsec) = 0;
};

