#ifndef TCPKTRCODER_H
#define TCPKTRCODER_H

#include <Tcp/BaseTcpDeviceCoder.h>

#include <QFile>
#include <QPointF>
#include <QVector>
#include <QDateTime>
#include <QDataStream>

#include "TcpDefines.h"

class TcpKTRCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT

private:
	double m_altitude;
	QFile m_logFile;

public:
	explicit TcpKTRCoder(QObject* parent = NULL);
	virtual ~TcpKTRCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);

private:
	MessageSP parseLocationFromBoard(const QByteArray& data);
	MessageSP parseLocationFromKTR(const QByteArray& data);
	MessageSP parseBoardList(const QByteArray& data);
};

#endif // TCPKTRCODER_H
