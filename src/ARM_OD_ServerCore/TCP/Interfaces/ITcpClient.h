#ifndef ITCPCLIENT_H
#define ITCPCLIENT_H

#include <qglobal.h>
class QObject;

class ITcpClient
{

public:
	virtual ~ITcpClient() {}

	virtual void connectToHost(const QString& host, const quint32& port)	= 0;
	virtual void disconnectFromHost()	= 0;
	virtual bool isConnected()	= 0;
	virtual void writeData(const QByteArray& data)	= 0;
	virtual QString getHost()	= 0; // Concatenation of host, ":" and port
	virtual QObject* asQObject()	= 0;
};

#endif // ITCPCLIENT_H
