#ifndef ARM_OD_SRV_H
#define ARM_OD_SRV_H

#include <QObject>
#include <QTextCodec>

#include <QMetaType>

#include <QPointF>

#include "RPC/RPCServer.h"
#include "TCP/TCPController.h"

#include "Common/Router.h"
#include "Common/Subscriber.h"

#include "TCP/UAV/BLAController.h"

#include "Common/Simulators/AtlantSimulator.h"

#include "RPC/Client/RPCClient_R.h"

#include "TCP/NIIPP/NIIPPController.h"

#include "AIS/Ais.h"

#include "TCP/TcpManager.h"

typedef QSharedPointer<IMessageOld> rpc_msg;
typedef QVector<QPointF>         rpc_send_points_vector;
typedef QPointF                  rpc_QPointF;
typedef QByteArray               rpc_send_ba_data;

class ARM_OD_Srv : public QObject
{
	Q_OBJECT
public:
	ARM_OD_Srv();
	~ARM_OD_Srv();

public:

private:
	RPCServer*      _rpc_server;
	Router*         _router;
	Subscriber*     _subscriber_up;
	TCPController*  _tcp_controller;
	RPCClient_R*    _rpc_client1;
	NIIPPController*    _niipp1;
	NIIPPController*    _niipp2;

	Ais*             _ais;

	QString         _ip_ktr;
	quint16         _port_ktr;

private:
	void _read_settings();
	void _read_settingsKTR();


signals:
	void signalStartRPC();
	void signalStopRPC();
	void signalFinishRPC();

	void closeALL();

};

Q_DECLARE_METATYPE(rpc_msg)
Q_DECLARE_METATYPE(rpc_QPointF)
Q_DECLARE_METATYPE(rpc_send_ba_data)

//Q_DECLARE_METATYPE(quint32)
//Q_DECLARE_METATYPE(rpc_send_points_vector)

#endif // ARM_OD_SRV_H
