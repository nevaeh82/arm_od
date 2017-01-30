#ifndef PRMSIMULATOR_H
#define PRMSIMULATOR_H

#include <QMap>
#include <QTimer>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>

#include <RPC/RpcServerBase.h>

#include <zavCommon/UAVDefines.h>

#define RPC_SLOT_SERVER_SEND_BPLA_DEF          "rpc_slot_server_send_bpla_def"
#define RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO     "rpc_slot_server_send_bpla_def_auto"
#define RPC_SLOT_SERVER_SEND_BPLA_DEF_SINGLE   "rpc_slot_server_send_bpla_def_single"
#define RPC_SLOT_SERVER_SEND_HYPERBOLA         "rpc_slot_server_send_hyperbola"

class PrmSimulator : public RpcServerBase
{
	Q_OBJECT

private:
	uint m_port;

	QTimer m_updateTimer;

	int mode;
	double centerLat;
	double centerLon;
	int alt;
	int zone;
	int bearing;
	double angle;
	double frequency;

    UAVPositionDataEnemy encodeBplaData(const QDateTime &time = QDateTime::currentDateTime());

	void sendUavsData();
	void sendHyperbolas();

public:
	explicit PrmSimulator( const uint& port, QObject *parent = 0 );
	virtual ~PrmSimulator();

	bool start( quint16 port = 0, QHostAddress ipAddress = QHostAddress::Any );

protected slots:
	void update();
};

#endif // PRMSIMULATOR_H
