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


#define RPC_SLOT_SET_CLIENT_ID   "rpc_slot_set_client_id"

///server
#define RPC_SLOT_SERVER_SEND_BLA_POINTS               "rpc_slot_server_send_bla_points"

/// AIS
#define RPC_SLOT_SERVER_SEND_AIS_DATA                 "rpc_slot_server_send_ais_data"

/// from ARM_R
#define RPC_SLOT_SERVER_SEND_BPLA_POINTS              "rpc_slot_server_send_bpla_point_from_arm_r"
#define RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO         "rpc_slot_server_send_bpla_point_from_arm_r_auto"

/// NIIPP
#define RPC_SLOT_SERVER_SEND_NIIPP_DATA                "rpc_slot_server_send_NIIPP_data"

/// to NIIPP
#define RPC_SLOT_SET_NIIPP_BPLA                         "rpc_slot_set_niipp_data"

/// to SOLVER
#define RPC_SLOT_SET_SOLVER_DATA    "rpc_slot_set_solver_data"
#define RPC_SLOT_SET_SOLVER_CLEAR   "rpc_slot_set_solver_clear"

/// from ARM_R Atlant direction
#define RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION           "rpc_slot_server_send_atlant_direction"
#define RPC_SLOT_SERVER_SEND_ATLANT_POSITION            "rpc_slot_server_send_atlant_position"

///PRM to RPC client
//#define RPC_SLOT_SERVER_PRM_STATUS                   "rpc_slot_server_prm_status"

class RPCServer : public RpcServerBase, public IRPC
{
    Q_OBJECT
public:
	RPCServer(QObject *parent = NULL);
    ~RPCServer();

public:
	bool start(quint16 port, QHostAddress address);

   // virtual int start();
  //  virtual int stop();
	quint64 get_client_id(IClient* client);

	void setRouter(IRouter*);

	void sendDataByRpc(const QString& signalType, const QByteArray& data);

private slots:
    void _slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
    void _slotRPCConnetion(quint64 client);
    void _slotRPCDisconnected(quint64 client);

public slots:
    void rpc_slot_set_client_id(quint64 client, int id);

    void rpc_slot_set_niipp_data(quint64 client, QByteArray data);

    void rpc_slot_set_solver_data(quint64 client, QByteArray data);
    void rpc_slot_set_solver_clear(quint64 client, QByteArray data);

    /// BLA
    void rpc_slot_send_bla_points(quint64 client, int id, rpc_QPointF points, double alt, double speed, double course, int state);

    /// AIS
    void rpc_slot_send_ais_data(quint64 client, QByteArray* data);

    /// ARM_R_Server
    void rpc_slot_send_bpla_points(quint64 client, QByteArray* data);
    void rpc_slot_send_bpla_points_auto(quint64 client, QByteArray* data);

    void rpc_slot_send_atlant_direction(quint64 client, QByteArray* data);
    void rpc_slot_send_atlant_position(quint64 client, QByteArray* data);

    /// NIIPP
    void rpc_slot_send_NIIPP_data(quint64 client, QByteArray* data);

private:
    QxtRPCPeer*     _rpc_server;

    QMap<quint64, IClient *> _map_clients;

    IRouter*        _router;
	ISubscriber*    _subscriber;

signals:
    void finished();

};

#endif // RPCSERVER_H
