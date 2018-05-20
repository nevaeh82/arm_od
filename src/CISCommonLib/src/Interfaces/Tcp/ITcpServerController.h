#ifndef ITCPSERVERCONTROLLER_H
#define ITCPSERVERCONTROLLER_H

#include <QHostAddress>

#include <qglobal.h>
#include "Templates/MessageSP.h"

class ITcpServerController
{

public:
	virtual ~ITcpServerController() {}

	virtual void createTcpServer()	= 0;
	virtual void createTcpServerCoder()	= 0;

	virtual void start(const QHostAddress& host,  const quint16 port) = 0;
	virtual void stop()	= 0;

	virtual bool isListening()	= 0;

	virtual bool sendData(const uint clientId, const MessageSP message) = 0;
	virtual bool sendData(const MessageSP message) = 0;

	virtual QObject* asQObject()	= 0;
};

#endif // ITCPSERVERCONTROLLER_H
