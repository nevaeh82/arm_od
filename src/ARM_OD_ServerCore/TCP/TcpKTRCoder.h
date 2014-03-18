#ifndef TCPKTRCODER_H
#define TCPKTRCODER_H

#include "BaseClasses/BaseTcpDeviceCoder.h"

class TcpKTRCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT

public:
	explicit TcpKTRCoder(QObject* parent = NULL);
	virtual ~TcpKTRCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();
};

#endif // TCPKTRCODER_H
