#ifndef ITCPDEVICECONTROLLER_H
#define ITCPDEVICECONTROLLER_H

#include <qglobal.h>
#include "MessageSP.h"

class ITcpDeviceController
{

public:
	virtual ~ITcpDeviceController() {}

	virtual void createTcpClient()	= 0;
	virtual void createTcpDeviceCoder()	= 0;

	virtual void connectToHost(const QString& host, const quint32& port)	= 0;
	virtual void disconnectFromHost()	= 0;

	virtual bool isConnected()	= 0;
	virtual QString getHost()	= 0; // Concatenation of host, ":" and port

	virtual void sendData(const MessageSP message)	= 0;
	virtual QObject* asQObject()	= 0;
};

#endif // ITCPDEVICECONTROLLER_H
