#ifndef TCPNIIPPCODER_H
#define TCPNIIPPCODER_H

#include <Tcp/BaseTcpDeviceCoder.h>

#include <QDateTime>
#include <QPointF>
#include <QDataStream>
#include <QStringList>

#include "TcpDefines.h"

class TcpNIIPPCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT

public:
	explicit TcpNIIPPCoder(QObject* parent = NULL);
	virtual ~TcpNIIPPCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);

private:
	QByteArray prepareDataToSend(const QStringList& list);
	quint8 crc(const QByteArray& data);
};

#endif // TCPNIIPPCODER_H
