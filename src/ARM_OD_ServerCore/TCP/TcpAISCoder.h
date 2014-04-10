#ifndef TCPAISCODER_H
#define TCPAISCODER_H

#include <Tcp/BaseTcpDeviceCoder.h>

#include <QList>
#include <QVector>
#include <QDataStream>

#include "TcpDefines.h"
#include "TcpAISZoneManager.h"

class TcpAISCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT

private:
	ITcpAISZoneManager* m_zoneManager;

	/// Brackets and commas coords in incoming data
	struct Bracketed {
		int br1;
		int br2;
		int comma[17];
	};

	QList<Bracketed> m_listBr;

public:
	explicit TcpAISCoder(ITcpAISZoneManager* zoneManager, QObject* parent = NULL);
	virtual ~TcpAISCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);

private:
	MessageSP filterData(const QString& data);
};

#endif // TCPAISCODER_H
