#ifndef TCPNIIPPCODER_H
#define TCPNIIPPCODER_H

#include "BaseClasses/BaseTcpDeviceCoder.h"

#include <QDateTime>
#include <QPointF>
#include <QDataStream>
#include <QStringList>

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
	virtual QObject* asQObject();

private:
	QByteArray prepareDataToSend(const QStringList& list);
	quint8 crc(const QByteArray& data);
};

#endif // TCPNIIPPCODER_H
