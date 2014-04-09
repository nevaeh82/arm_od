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

	//! Координаты кавычек и запятых в полученных данных
	struct Bracketed {
		int br1;
		int br2;
		int comma[17];
	};

	//! DataFly содержит характеристики самолета
	struct DataFly {
		QString height;
		QString latitute;
		QString longinude;
		QString speed;
		QString namePlane;
		QString course;
	};

	QList<Bracketed> listBr;
	QVector<DataFly> listDataFly;

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
