#ifndef BASETCPDEVICECODER_H
#define BASETCPDEVICECODER_H

#include <QObject>
#include <PwLogger/PwLogger.h>

#include "../Interfaces/ITcpDeviceCoder.h"
#include "../TcpDefines.h"

class BaseTcpDeviceCoder : public QObject, public ITcpDeviceCoder
{
	Q_OBJECT

protected:
	Pw::Logger::ILogger* m_logger;

public:
	explicit BaseTcpDeviceCoder(Pw::Logger::ILogger* logger, QObject* parent = NULL);
	virtual ~BaseTcpDeviceCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();
};

#endif // BASETCPDEVICECODER_H
