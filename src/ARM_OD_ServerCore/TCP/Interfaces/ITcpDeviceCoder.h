#ifndef ITCPDEVICECODER_H
#define ITCPDEVICECODER_H

#include <QSharedPointer>

#include "MessageSP.h"

class ITcpDeviceCoder
{

public:
	virtual ~ITcpDeviceCoder() {}

	virtual MessageSP encode(const QByteArray& data)	= 0;
	virtual QByteArray decode(const MessageSP message)	= 0;
	virtual QObject* asQObject()	= 0;
};

#endif // ITCPDEVICECODER_H
