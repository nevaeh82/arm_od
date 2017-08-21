#include "RPCServer.h"

#include <QDebug>
#include <QThread>

RPCServer::RPCServer(QObject* parent)
	: RpcServerBase(parent)
	, _router(0)
	, _subscriber(0)
	, m_armr_connection(false)
{
	connect(this, SIGNAL(sendDataByRpcSignal(QString,QByteArray)), this, SLOT(sendDataByRpcSlot(QString,QByteArray)));
}

RPCServer::~RPCServer()
{
}

bool RPCServer::start(quint16 port, QHostAddress address)
{
	connect(m_serverPeer, SIGNAL(clientConnected(quint64)), this, SLOT(_slotRPCConnetion(quint64)));
	connect(m_serverPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(_slotErrorRPCConnection(QAbstractSocket::SocketError)));
	connect(m_serverPeer, SIGNAL(clientDisconnected(quint64)), this, SLOT(_slotRPCDisconnected(quint64)));

	m_serverPeer->attachSlot(RPC_SLOT_SET_CLIENT_ID, this, SLOT(rpc_slot_set_client_id(quint64,int)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_NIIPP_BPLA, this, SLOT(rpc_slot_set_niipp_data(quint64,QByteArray)));
	m_serverPeer->attachSlot(RPC_SLOT_GET_NIIPP_CONNECTION_STATUS, this, SLOT(rpc_slot_get_niipp_connection_status(quint64,QByteArray)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_SOLVER_DATA, this, SLOT(rpc_slot_set_solver_data(quint64, QByteArray)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_SOLVER_CLEAR, this, SLOT(rpc_slot_set_solver_clear(quint64,QByteArray)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_ADSB_ENABLE, this, SLOT(rpc_slot_set_adsb_enable(quint64,QByteArray)));

	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCBPLAPoints(QByteArray)), RPC_SLOT_SERVER_SEND_BPLA_POINTS);
	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCBPLAPoints1(QByteArray)), RPC_SLOT_SERVER_SEND_BPLA_POINTS_1);
	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCMapPing(QByteArray)), RPC_SLOT_SERVER_SEND_MAP_PING);
    m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCNIIPPAlarm(QByteArray)), RPC_METHOD_NIIPP_WORK_STATUS);
	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCBPLAPointsAuto(QByteArray)), RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO);
	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCHyperbola(QByteArray)), RPC_SLOT_SERVER_SEND_HYPERBOLA);

	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCARMRConnection(bool)), RPC_SLOT_SERVER_SEND_ARMR_CONNECTION);

	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCAtlantDirection(QByteArray)), RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION);
	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCAtlantPosition(QByteArray)), RPC_SLOT_SERVER_SEND_ATLANT_POSITION);

	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCBLAPoints(QByteArray)), RPC_SLOT_SERVER_SEND_BLA_POINTS);
	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCAISData(QByteArray)), RPC_SLOT_SERVER_SEND_AIS_DATA);
	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCNIIPPData(QByteArray)), RPC_SLOT_SERVER_SEND_NIIPP_DATA);
	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCNIIPPConnectionStatus(QByteArray)), RPC_SLOT_SERVER_SEND_NIIPP_CONNECTION_STATUS);

	m_serverPeer->attachSignal(this, SIGNAL(signalSendToRPCADSBData(QByteArray)), RPC_SLOT_SERVER_SEND_ADSB_DATA);

	// Signals and slots for config query
	m_serverPeer->attachSlot(RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST, this, SLOT(requestGetStationListSlot(quint64,QString)));
	m_serverPeer->attachSlot(RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION, this, SLOT(requestGetDbConfigurationSlot(quint64,QString)));
//	m_serverPeer->attachSignal(this, SIGNAL(), RPC_METHOD_CONFIG_ANSWER_STATION_LIST);
//	m_serverPeer->attachSignal(this, SIGNAL(), RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION);

	return RpcServerBase::start(port, address);
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
	//    connect(cl, SIGNAL(signalSendToRPCBLAPoints(quint64,int,rpc_QPointF,double,double,double,int)), this, SLOT(rpc_slot_send_bla_points(quint64,int,rpc_QPointF,double,double,double,int)));
	connect(cl, SIGNAL(signalSendToRPCAISData(quint64,QByteArray*)), this, SLOT(rpc_slot_send_ais_data(quint64,QByteArray*)));
	connect(cl, SIGNAL(signalSendToRPCBPLAPoints(quint64,QByteArray*)), this, SLOT(rpc_slot_send_bpla_points(quint64,QByteArray*)));
	connect(cl, SIGNAL(signalSendToRPCBPLAPoints1(quint64,QByteArray*)), this, SLOT(rpc_slot_send_bpla_points_1(quint64,QByteArray*)));
	connect(cl, SIGNAL(signalSendToRPCMapPing(quint64,QByteArray*)), this, SLOT(rpc_slot_send_map_ping(quint64,QByteArray*)));
    connect(cl, SIGNAL(signalSendToRPCNIIPPAlarm(quint64,QByteArray*)), this, SLOT(rpc_slot_send_niipp_alarm(quint64,QByteArray*)));
	connect(cl, SIGNAL(signalSendToRPCBPLAPointsAuto(quint64,QByteArray*)), this, SLOT(rpc_slot_send_bpla_points_auto(quint64,QByteArray*)));
	connect(cl, SIGNAL(signalSendToRPCBPLAPointsSingle(quint64,QByteArray*)), this, SLOT(rpc_slot_send_bpla_points_single(quint64,QByteArray*)));
	connect(cl, SIGNAL(signalSendToRPCHyperbola(quint64,QByteArray*)), this, SLOT(rpc_slot_send_hyperbola(quint64,QByteArray*)));

	connect(cl, SIGNAL(signalSendToNIIPPPoints(quint64,QByteArray*)), this, SLOT(rpc_slot_send_NIIPP_data(quint64,QByteArray*)));
	connect(cl, SIGNAL(signalSendToRPCAtlantDirection(quint64,QByteArray*)), this, SLOT(rpc_slot_send_atlant_direction(quint64,QByteArray*)));
	connect(cl, SIGNAL(signalSendToRPCAtlantPosition(quint64,QByteArray*)), this, SLOT(rpc_slot_send_atlant_position(quint64,QByteArray*)));

	emit signalSendToRPCARMRConnection(m_armr_connection);
	time.singleShot(5000, this, SLOT(slotSendStatusARMR()));
}


void RPCServer::_slotRPCDisconnected(quint64 client)
{
	IClient *cl = _map_clients.value(client);
	_subscriber->remove_subscription(cl);
	delete cl;

	_map_clients.remove(client);
}

void RPCServer::requestGetStationListSlot(quint64 client, QString configFilename)
{
	foreach (IRpcListener* receiver, m_receiversList) {
		receiver->onMethodCalled(RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST, QVariant(configFilename));
	}
}

void RPCServer::requestGetDbConfigurationSlot(quint64 client, QString configFilename)
{
	foreach (IRpcListener* receiver, m_receiversList) {
		receiver->onMethodCalled(RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION, QVariant(configFilename));
	}
}

void RPCServer::slotARMRSetConnection(bool b)
{
	m_armr_connection = b;
}

void RPCServer::slotSendStatusARMR()
{
	emit signalSendToRPCARMRConnection(m_armr_connection);
}

void RPCServer::sendDataByRpcSlot(QString signalType, QByteArray data)
{
	if (signalType == RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION) {
		emit signalSendToRPCAtlantDirection(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_ATLANT_POSITION) {
		emit signalSendToRPCAtlantPosition(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_BPLA_POINTS) {
		emit signalSendToRPCBPLAPoints(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_BPLA_POINTS_1) {
		emit signalSendToRPCBPLAPoints1(data);
	}
	else if (signalType == RPC_SLOT_SERVER_SEND_MAP_PING) {
			emit signalSendToRPCMapPing(data);
	}
    else if (signalType == RPC_METHOD_NIIPP_WORK_STATUS) {
            emit signalSendToRPCNIIPPAlarm(data);
    }
	else if (signalType == RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO) {
		emit signalSendToRPCBPLAPointsAuto(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_BPLA_POINTS_SINGLE) {
		emit signalSendToRPCBPLAPointsSingle(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_BLA_POINTS) {
		emit signalSendToRPCBLAPoints(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_HYPERBOLA) {
		emit signalSendToRPCHyperbola(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_AIS_DATA) {
		emit signalSendToRPCAISData(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_NIIPP_DATA) {
		emit signalSendToRPCNIIPPData(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_NIIPP_CONNECTION_STATUS) {
		emit signalSendToRPCNIIPPConnectionStatus(data);
	} else if (signalType == RPC_SLOT_SERVER_SEND_ADSB_DATA) {
		emit signalSendToRPCADSBData(data);
	}
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
	cl->setId(id);
	cl->setType(1);
	qDebug() << "set id = " << id << "for client = " << client;

	/// added subscription with type == 1001
	_subscriber->add_subscription(KTR_BLA, cl);
	_subscriber->add_subscription(ARM_R_SERVER_BPLA_COORDS, cl);
	_subscriber->add_subscription(ARM_R_SERVER_BPLA_COORDS_1, cl);
	_subscriber->add_subscription(ARM_R_SERVER_MAP_PING, cl);
    _subscriber->add_subscription(ARM_R_SERVER_NIIPALARM, cl);
	_subscriber->add_subscription(ARM_R_SERVER_BPLA_COORDS_AUTO, cl);
	_subscriber->add_subscription(ARM_R_SERVER_BPLA_COORDS_SINGLE, cl);
	_subscriber->add_subscription(ARM_R_SERVER_HYPERBOLA, cl);
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
	QByteArray ba;
	ba.append(data);

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_NIIPP_BPLA, QVariant(ba));
	}


//	QSharedPointer<IMessageOld> msg(new MessageOld(id, NIIPP_BPLA, ba));
	//	_subscriber->data_ready(NIIPP_BPLA, msg);
}

void RPCServer::rpc_slot_get_niipp_connection_status(quint64 client, QByteArray data)
{
	QDataStream ds(&data, QIODevice::ReadOnly);
	int id = -1;
	ds >> id;
	QByteArray ba;
	ba.append(data);

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_GET_NIIPP_CONNECTION_STATUS, QVariant(ba));
	}
}

void RPCServer::rpc_slot_set_solver_data(quint64 client, QByteArray data)
{
	/*TODO: REMOVE subscriber usage*/
	/*QDataStream ds(&data, QIODevice::ReadOnly);
	int id = -1;
	ds >> id;

	QByteArray* ba = new QByteArray();
	ba->append(data);

	QSharedPointer<IMessageOld> msg(new MessageOld(id, SOLVER_SET, ba));
	_subscriber->data_ready(SOLVER_SET, msg);*/

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_SOLVER_DATA, QVariant(data));
	}
}

void RPCServer::rpc_slot_set_solver_clear(quint64 client, QByteArray data)
{
	/*TODO: REMOVE subscriber usage*/
	/*QDataStream ds(&data, QIODevice::ReadOnly);
	int id = -1;
	ds >> id;


	QByteArray* ba = new QByteArray();
	ba->append(data);

	QSharedPointer<IMessageOld> msg(new MessageOld(id, SOLVER_CLEAR, ba));
	_subscriber->data_ready(SOLVER_CLEAR, msg);*/

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_SOLVER_CLEAR, QVariant(data));
	}
}

void RPCServer::rpc_slot_set_adsb_enable(quint64 client, QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_ADSB_ENABLE, QVariant(data));
	}
}

quint64 RPCServer::get_client_id(IClient *client)
{
	return _map_clients.key(client);
}

void RPCServer::setRouter(IRouter* router)
{
	_router = router;
	_subscriber = router->get_subscriber();
}



void RPCServer::sendDataByRpc(const QString &signalType, const QByteArray &data)
{
	QMutexLocker locker(&m_mutex);

	emit sendDataByRpcSignal(signalType, data);
}

///// send bpla coords
void RPCServer::rpc_slot_send_bla_points(quint64 client, int id, rpc_QPointF points, double alt, double speed, double course, int state)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_BLA_POINTS, QVariant::fromValue(id), QVariant::fromValue(points), QVariant::fromValue(alt), QVariant::fromValue(speed), QVariant::fromValue(course), QVariant::fromValue(state));
}

void RPCServer::rpc_slot_send_ais_data(quint64 client, QByteArray *data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_AIS_DATA, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_bpla_points(quint64 client, QByteArray* data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_BPLA_POINTS, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_bpla_points_1(quint64 client, QByteArray* data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_BPLA_POINTS_1, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_map_ping(quint64 client, QByteArray* data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_MAP_PING, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_niipp_alarm(quint64 client, QByteArray* data)
{
    m_serverPeer->call(client, RPC_METHOD_NIIPP_WORK_STATUS, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_bpla_points_auto(quint64 client, QByteArray *data)
{
	m_serverPeer->call( RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_bpla_points_single(quint64 client, QByteArray *data)
{
	m_serverPeer->call( RPC_SLOT_SERVER_SEND_BPLA_POINTS_SINGLE, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_hyperbola(quint64 client, QByteArray *data)
{
	m_serverPeer->call( RPC_SLOT_SERVER_SEND_HYPERBOLA, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_atlant_direction(quint64 client, QByteArray *data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_atlant_position(quint64 client, QByteArray *data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_ATLANT_POSITION, QVariant::fromValue(*data));
}
void RPCServer::rpc_slot_send_NIIPP_data(quint64 client, QByteArray *data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_NIIPP_DATA, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_NIIPP_connection_status(quint64 client, QByteArray* data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_NIIPP_CONNECTION_STATUS, QVariant::fromValue(*data));
}
