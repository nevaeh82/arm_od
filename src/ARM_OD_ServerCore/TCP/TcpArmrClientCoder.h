#ifndef TCPARMRCLIENTCODER_H
#define TCPARMRCLIENTCODER_H

#include <QObject>
#include "Tcp/BaseTcpDeviceCoder.h"
#include "Protobuf/ARMR_OD/ZVPacket.pb.h"

class TcpArmrClientCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT
public:
	explicit TcpArmrClientCoder(QObject *parent = NULL);
	virtual ~TcpArmrClientCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();
	
};

#endif // TCPARMRCLIENTCODER_H