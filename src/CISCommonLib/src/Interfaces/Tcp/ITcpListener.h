#ifndef ITCPLISTENER_H
#define ITCPLISTENER_H

#include "Templates/MessageSP.h"

class ITcpListener
{

public:
	virtual ~ITcpListener() {}

	virtual void onMessageReceived(const quint32 deviceType,
								   const QString& device,
								   const MessageSP argument)	= 0;
};

#endif // ITCPLISTENER_H
