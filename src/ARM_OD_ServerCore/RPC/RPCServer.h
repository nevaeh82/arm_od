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

public slots:
    void rpc_slot_set_client_id(quint64 client, int id);

    void rpc_slot_set_niipp_data(quint64 client, QByteArray data);

    void rpc_slot_set_solver_data(quint64 client, QByteArray data);
    void rpc_slot_set_solver_clear(quint64 client, QByteArray data);

    /// AIS
    void rpc_slot_send_ais_data(quint64 client, QByteArray* data);

    /// NIIPP
    void rpc_slot_send_NIIPP_data(quint64 client, QByteArray* data);

private:
    QMap<quint64, IClient *> _map_clients;

    IRouter*        _router;
	ISubscriber*    _subscriber;

signals:
    void finished();

	void signalSendToRPCBPLAPoints(QByteArray);
	void signalSendToRPCBPLAPointsAuto(QByteArray);
	void signalSendToRPCAtlantDirection(QByteArray);
	void signalSendToRPCAtlantPosition(QByteArray);

	void signalSendToRPCBLAPoints(QByteArray);
	void signalSendToRPCAISData(QByteArray);
	void signalSendToRPCNIIPPData(QByteArray);

};

#endif // RPCSERVER_H
