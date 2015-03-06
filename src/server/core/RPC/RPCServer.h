#ifndef RPCSERVER_H
#define RPCSERVER_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>

#include <QTextCodec>
#include <QSettings>
#include <QStringList>

#include <QDataStream>

#include <QMap>


#include "IRPC.h"
#include "RPCClient.h"
#include "../Common/IRouter.h"
#include "../Common/Subscriber.h"

#include "Message.h"

#include "Client/RPCClient_R.h"

#include "Rpc/RpcServerBase.h"
#include "Rpc/RpcDefines.h"

#include <QMutexLocker>
#include <QMutex>

class RPCServer : public RpcServerBase, public IRPC
{
	Q_OBJECT
public:
	RPCServer(QObject *parent = NULL);
	~RPCServer();

public:
	bool start(quint16 port, QHostAddress address);
	quint64 get_client_id(IClient* client);

	void setRouter(IRouter*);

	void sendDataByRpc(const QString& signalType, const QByteArray& data);

private slots:
	void _slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
	void _slotRPCConnetion(quint64 client);
	void _slotRPCDisconnected(quint64 client);

	void sendDataByRpcSlot(QString signalType, QByteArray data);
	void requestGetStationListSlot(quint64 client, QString configFilename);
	void requestGetDbConfigurationSlot(quint64 client, QString configFilename);


public slots:
	void rpc_slot_set_client_id(quint64 client, int id);

	void rpc_slot_set_niipp_data(quint64 client, QByteArray data);
	void rpc_slot_get_niipp_connection_status(quint64 client, QByteArray data);

	void rpc_slot_set_solver_data(quint64 client, QByteArray data);
	void rpc_slot_set_solver_clear(quint64 client, QByteArray data);

	/// BLA
	void rpc_slot_send_bla_points(quint64 client, int id, rpc_QPointF points, double alt, double speed, double course, int state);

	/// AIS
	void rpc_slot_send_ais_data(quint64 client, QByteArray* data);

	/// ARM_R_Server
	void rpc_slot_send_bpla_points(quint64 client, QByteArray* data);
	void rpc_slot_send_bpla_points_auto(quint64 client, QByteArray* data);
	void rpc_slot_send_bpla_points_single(quint64 client, QByteArray* data);
	void rpc_slot_send_hyperbola(quint64 client, QByteArray* data);

	void rpc_slot_send_atlant_direction(quint64 client, QByteArray* data);
	void rpc_slot_send_atlant_position(quint64 client, QByteArray* data);


	/// NIIPP
	void rpc_slot_send_NIIPP_data(quint64 client, QByteArray* data);
	void rpc_slot_send_NIIPP_connection_status(quint64 client, QByteArray* data);

private:
	QMap<quint64, IClient *> _map_clients;

	IRouter*        _router;
	ISubscriber*    _subscriber;

	QMutex m_mutex;

signals:
	void finished();

	void signalSendToRPCBPLAPoints(QByteArray);
	void signalSendToRPCBPLAPointsAuto(QByteArray);
	void signalSendToRPCBPLAPointsSingle(QByteArray);
	void signalSendToRPCHyperbola(QByteArray);
	void signalSendToRPCAtlantDirection(QByteArray);
	void signalSendToRPCAtlantPosition(QByteArray);

	void signalSendToRPCBLAPoints(QByteArray);
	void signalSendToRPCAISData(QByteArray);
	void signalSendToRPCNIIPPData(QByteArray);
	void signalSendToRPCNIIPPConnectionStatus(QByteArray);

	void sendDataByRpcSignal(QString signalType, QByteArray data);

};

#endif // RPCSERVER_H
