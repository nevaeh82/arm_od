#ifndef BASETCPDEVICECODER_H
#define BASETCPDEVICECODER_H

#include <QObject>

#include "Interfaces/Tcp/ITcpDeviceCoder.h"

#include "Logger/Logger.h"

#define TCP_EMPTY_MESSAGE "tcpEmptyMessage"

class BaseTcpDeviceCoder : public QObject, public ITcpDeviceCoder
{
	Q_OBJECT

public:
	explicit BaseTcpDeviceCoder(QObject* parent = NULL);
	virtual ~BaseTcpDeviceCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();
	virtual bool isAll();
};

#endif // BASETCPDEVICECODER_H
