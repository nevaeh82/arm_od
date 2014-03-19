#include "RPCServer.h"

#include <QDebug>

RPCServer::RPCServer(IRouter* router)
{
    _rpc_server = NULL;
    _router = router;
	_subscriber = router->get_subscriber();
}

RPCServer::~RPCServer()
{
}

int RPCServer::start()
{
    _rpc_server = new QxtRPCPeer();
    connect(_rpc_server, SIGNAL(clientConnected(quint64)), this, SLOT(_slotRPCConnetion(quint64)));
    connect(_rpc_server, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(_slotErrorRPCConnection(QAbstractSocket::SocketError)));
    connect(_rpc_server, SIGNAL(clientDisconnected(quint64)), this, SLOT(_slotRPCDisconnected(quint64)));

    _rpc_server->attachSlot(RPC_SLOT_SET_CLIENT_ID, this, SLOT(rpc_slot_set_client_id(quint64,int)));
    _rpc_server->attachSlot(RPC_SLOT_SET_NIIPP_BPLA, this, SLOT(rpc_slot_set_niipp_data(quint64,QByteArray)));
    _rpc_server->attachSlot(RPC_SLOT_SET_SOLVER_DATA, this, SLOT(rpc_slot_set_solver_data(quint64, QByteArray)));
	_rpc_server->attachSlot(RPC_SLOT_SET_SOLVER_CLEAR, this, SLOT(rpc_slot_set_solver_clear(quint64,QByteArray)));

    if(!_rpc_server->listen(QHostAddress::Any, 24550))
    {
        qDebug() << "error";
    }
    return 0;
}

/// slot if have some error while connetiting
void RPCServer::_slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Have error in connection ";
    QString thiserror;
    switch(socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        thiserror.append(("������! ���������� � ������� ��������!"));
        return;
        break;
    case QAbstractSocket::HostNotFoundError:
        thiserror.append(("������! �� ������� ������������ � ������!"));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        thiserror.append(("������! �������� � ����������"));
        break;
    default:
//        thiserror.append(("������! ��������� ������: " + _rpc_client->->errorString()));
        break;
    }
    qDebug() << thiserror;
}

/// slot when connection complete
void RPCServer::_slotRPCConnetion(quint64 client)
{
    qDebug() << "Client connection" << client;
    if(_map_clients.contains(client))
    {
        delete _map_clients.value(client);
    }
    RPCClient* cl = new RPCClient(_router, this);
    _map_clients.insert(client, cl);
    connect(cl, SIGNAL(signalSendToRPCBLAPoints(quint64,int,rpc_QPointF,double,double,double,int)), this, SLOT(rpc_slot_send_bla_points(quint64,int,rpc_QPointF,double,double,double,int)));
    connect(cl, SIGNAL(signalSendToRPCAISData(quint64,QByteArray*)), this, SLOT(rpc_slot_send_ais_data(quint64,QByteArray*)));
    connect(cl, SIGNAL(signalSendToRPCBPLAPoints(quint64,QByteArray*)), this, SLOT(rpc_slot_send_bpla_points(quint64,QByteArray*)));
    connect(cl, SIGNAL(signalSendToRPCBPLAPointsAuto(quint64,QByteArray*)), this, SLOT(rpc_slot_send_bpla_points_auto(quint64,QByteArray*)));

    connect(cl, SIGNAL(signalSendToNIIPPPoints(quint64,QByteArray*)), this, SLOT(rpc_slot_send_NIIPP_data(quint64,QByteArray*)));
    connect(cl, SIGNAL(signalSendToRPCAtlantDirection(quint64,QByteArray*)), this, SLOT(rpc_slot_send_atlant_direction(quint64,QByteArray*)));
    connect(cl, SIGNAL(signalSendToRPCAtlantPosition(quint64,QByteArray*)), this, SLOT(rpc_slot_send_atlant_position(quint64,QByteArray*)));

}

void RPCServer::_slotRPCDisconnected(quint64 client)
{
    IClient *cl = _map_clients.value(client);
    _subscriber->remove_subscription(cl);
    delete cl;

    _map_clients.remove(client);
}

/// add client identification to QMap for assosiating with client sender
void RPCServer::rpc_slot_set_client_id(quint64 client, int id)
{
    if(!_map_clients.contains(client))
    {
        return;
        //delete _map_clients.value(client);
    }
    IClient *cl = _map_clients.value(client);
    cl->set_id(id);
    cl->set_type(1);
    qDebug() << "set id = " << id << "for client = " << client;

    /// added subscription with type == 1001
    _subscriber->add_subscription(KTR_BLA, cl);
    _subscriber->add_subscription(ARM_R_SERVER_BPLA_COORDS, cl);
    _subscriber->add_subscription(ARM_R_SERVER_BPLA_COORDS_AUTO, cl);
    _subscriber->add_subscription(NIIPP_ANSWER, cl);
    _subscriber->add_subscription(ARM_R_SERVER_ATLANT_DIRECTION, cl);
    _subscriber->add_subscription(ARM_R_SERVER_ATLANT_POSITION, cl);

	_subscriber->add_subscription(AIS_DATA, cl);
}

void RPCServer::rpc_slot_set_niipp_data(quint64 client, QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);
    int id = -1;
    ds >> id;
    QByteArray* ba = new QByteArray();
    ba->append(data);

	QSharedPointer<IMessageOld> msg(new MessageOld(id, NIIPP_BPLA, ba));
    _subscriber->data_ready(NIIPP_BPLA, msg);
}

void RPCServer::rpc_slot_set_solver_data(quint64 client, QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);
    int id = -1;
    ds >> id;

    QByteArray* ba = new QByteArray();
    ba->append(data);

	QSharedPointer<IMessageOld> msg(new MessageOld(id, SOLVER_SET, ba));
    _subscriber->data_ready(SOLVER_SET, msg);
}

void RPCServer::rpc_slot_set_solver_clear(quint64 client, QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);
    int id = -1;
    ds >> id;


    QByteArray* ba = new QByteArray();
    ba->append(data);

	QSharedPointer<IMessageOld> msg(new MessageOld(id, SOLVER_CLEAR, ba));
    _subscriber->data_ready(SOLVER_CLEAR, msg);
}

int RPCServer::stop()
{
    if(_rpc_server != NULL)
    {
        delete _rpc_server;
    }
    return 0;
}

quint64 RPCServer::get_client_id(IClient *client)
{
    return _map_clients.key(client);
}

///// send bpla coords
void RPCServer::rpc_slot_send_bla_points(quint64 client, int id, rpc_QPointF points, double alt, double speed, double course, int state)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_BLA_POINTS, QVariant::fromValue(id), QVariant::fromValue(points), QVariant::fromValue(alt), QVariant::fromValue(speed), QVariant::fromValue(course), QVariant::fromValue(state));
}

void RPCServer::rpc_slot_send_ais_data(quint64 client, QByteArray *data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_AIS_DATA, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_bpla_points(quint64 client, QByteArray* data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_BPLA_POINTS, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_bpla_points_auto(quint64 client, QByteArray *data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_atlant_direction(quint64 client, QByteArray *data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_atlant_position(quint64 client, QByteArray *data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_ATLANT_POSITION, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_NIIPP_data(quint64 client, QByteArray *data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_NIIPP_DATA, QVariant::fromValue(*data));
}

void RPCServer::aboutToQuitApp()
{
}
