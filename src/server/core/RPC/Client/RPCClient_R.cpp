#include <QDebug>

#include "TCP/TcpArmrClientCoder.h"
#include "RPCClient_R.h"

#include "SolverPacket1.pb.h"

RPCClient_R::RPCClient_R(IRouter* router, QObject *parent):
	RpcRoutedClient( RPC_METHOD_REGISTER_CLIENT, RPC_METHOD_DEREGISTER_CLIENT, parent )
{
	m_id = 701;
	m_type = 1;
	m_router = router;

	m_subscriber = m_router->get_subscriber();

	connect(this, SIGNAL(signalSendData(QSharedPointer<IMessageOld>)), this, SLOT(slotGetData(QSharedPointer<IMessageOld>)));
	connect(this, SIGNAL(signalSetCommand(IMessageOld*)), this, SLOT(slotSetCommand(IMessageOld*)));
}

RPCClient_R::~RPCClient_R()
{
}

void RPCClient_R::setId(int id)
{
	m_id = id;
}

int RPCClient_R::getId()
{
	return m_id;
}

void RPCClient_R::setType(int type)
{
	m_type = type;
}

int RPCClient_R::getType()
{
	return m_type;
}

void RPCClient_R::sendData(QSharedPointer<IMessageOld> msg_ptr)
{
	emit signalSendData(msg_ptr);
}

bool RPCClient_R::start(quint16 port, QHostAddress address)
{
	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SLOT(slotRCPConnetion()));
	connect(m_clientPeer, SIGNAL(connectedToServer()), SLOT(registerRoute()) );
	connect(m_clientPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(slotErrorRPCConnection(QAbstractSocket::SocketError)));

	connect(this, SIGNAL(signalSetCommand(IMessage*)), this, SLOT(slotSetCommand(IMessage*)));

	m_clientPeer->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetSolver(QByteArray)), RPC_SLOT_SET_DATA_TO_SOLVER);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetSolverClear(QByteArray)), RPC_SLOT_SET_CLEAR_TO_SOLVER);

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_DEF, this, SLOT(rpcSlotServerSendBplaDef(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_DEF_1, this, SLOT(rpcSlotServerSendBplaDef1(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_DEF, this, SLOT(rpcSlotServerSendBplaDef(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO, this, SLOT(rpcSlotServerSendBplaDefAuto(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_DEF_SINGLE, this, SLOT(rpcSlotServerSendBplaDefSingle(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_HYPERBOLA, this, SLOT(rpcSlotServerSendHyperbola(QByteArray)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_ATLANT_DIRECTION, this, SLOT(rpcSlotServerAtlantDirection(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_ATLANT_POSITION, this, SLOT(rpcSlotServerAtlantPosition(QByteArray)));

	log_debug("Start RPCClient_R");
	return RpcClientBase::start(port, address);
}

void RPCClient_R::setCommand(IMessageOld *msg)
{
	emit signalSetCommand(msg);
}

void RPCClient_R::pushMsg(QByteArray msg)
{
}

void RPCClient_R::registerRoute()
{
	RpcRoutedClient::registerRoute( 701 );
}

void RPCClient_R::sendDataByRpc(const QString& signalType, const QByteArray& data)
{
	if (signalType == TCP_ARMR_SEND_SOLVER_DATA){
		emit signalSetSolver(data);
	} else if (signalType == TCP_ARMR_SEND_SOLVER_CLEAR){
		emit signalSetSolverClear(data);
	}
}

void RPCClient_R::slotSetCommand(IMessageOld *msg)
{
	m_commandMsg = msg;
	formCommand(m_commandMsg);
}

void RPCClient_R::slotPushMsg(QByteArray msg)
{
	emit signalSetSolver(msg);
}

void RPCClient_R::slotGetData(QSharedPointer<IMessageOld> msg_ptr)
{
	/*int type1 = 1;
	int id = 0;
	IMessageOld *f = (msg_ptr.data());
	QByteArray* dd = f->get(id, type1);
	QDataStream ds(*dd);
	int id1 = 0;
	ds >> id1;
	int count = 0;
	ds >> count;
	double val = 0;
	ds >> val;
	QByteArray ba11;
	ba11.append(*dd);

	switch(type1)
	{
	case SOLVER_SET:
			emit signalSetSolver(ba11);
		break;
	case SOLVER_CLEAR:
			emit signalSetSolverClear(ba11);
		break;
	default:
		return;
		break;
	}*/
}

void RPCClient_R::formCommand(IMessageOld *msg)
{
}

/// slot when connection complete
void RPCClient_R::slotRCPConnetion()
{
	log_debug("Connection complete!");
	emit signalSetClientId(m_id);
}

void RPCClient_R::rpcSlotServerSendBplaDef(QByteArray ba)
{
	QByteArray *out = new QByteArray();
	out->append(ba);

	QSharedPointer<IMessageOld> msg(new MessageOld(m_id, ARM_R_SERVER_BPLA_COORDS, out));
	m_subscriber->data_ready(ARM_R_SERVER_BPLA_COORDS, msg);
}

void RPCClient_R::rpcSlotServerSendBplaDef1(QByteArray ba)
{
	QByteArray *out = new QByteArray();
	out->append(ba);

	QSharedPointer<IMessageOld> msg(new MessageOld(m_id, ARM_R_SERVER_BPLA_COORDS_1, out));
	m_subscriber->data_ready(ARM_R_SERVER_BPLA_COORDS_1, msg);
}

void RPCClient_R::rpcSlotServerSendBplaDefAuto(QByteArray ba)
{
	QByteArray *out = new QByteArray();
	out->append(ba);

	QSharedPointer<IMessageOld> msg(new MessageOld(m_id, ARM_R_SERVER_BPLA_COORDS_AUTO, out));
	m_subscriber->data_ready(ARM_R_SERVER_BPLA_COORDS_AUTO, msg);

}

void RPCClient_R::rpcSlotServerSendBplaDefSingle(QByteArray ba)
{
	QByteArray *out = new QByteArray();
	out->append(ba);

	QSharedPointer<IMessageOld> msg(new MessageOld(m_id, ARM_R_SERVER_BPLA_COORDS_SINGLE, out));
	m_subscriber->data_ready(ARM_R_SERVER_BPLA_COORDS_SINGLE, msg);

}

void RPCClient_R::rpcSlotServerSendHyperbola(QByteArray ba)
{
//	QByteArray inputData = ba;
//	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);

//	QList<QPointF> hyperbola;
//	double frequency;
//	inputDataStream >> hyperbola;
//	inputDataStream >> frequency;

	QByteArray *dataToSend = new QByteArray();
	dataToSend->append(ba);
	//QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	QSharedPointer<IMessageOld> msg(new MessageOld(m_id, ARM_R_SERVER_HYPERBOLA, dataToSend));
	m_subscriber->data_ready(ARM_R_SERVER_HYPERBOLA, msg);
}

void RPCClient_R::rpcSlotServerAtlantDirection(QByteArray ba1)
{
	QByteArray *ba = new QByteArray();
	ba->append(ba1);
//    qDebug() << "GOT FROM SERVER ATLANT! = ";// << id;

	QSharedPointer<IMessageOld> msg1(new MessageOld(m_id, ARM_R_SERVER_ATLANT_DIRECTION, ba));
	m_subscriber->data_ready(ARM_R_SERVER_ATLANT_DIRECTION, msg1);

}

void RPCClient_R::rpcSlotServerAtlantPosition(QByteArray ba1)
{
	QByteArray *ba = new QByteArray();
	ba->append(ba1);

	QSharedPointer<IMessageOld> msg1(new MessageOld(m_id, ARM_R_SERVER_ATLANT_POSITION, ba));
	m_subscriber->data_ready(ARM_R_SERVER_ATLANT_POSITION, msg1);

}

QByteArray RPCClient_R::encodeToEnemyUav(const QByteArray& data, bool singleMode)
{
	QByteArray inputData = data;
	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	UAVPositionDataEnemy uav;
	inputDataStream >> uav.time;
	inputDataStream >> uav.state;
	inputDataStream >> uav.latLonStdDev;
	inputDataStream >> uav.latLon;
	inputDataStream >> uav.speed;
	inputDataStream >> uav.altitude;
	inputDataStream >> uav.course;
	inputDataStream >> uav.frequency;
	dataStream << uav;

	if (singleMode) {
		inputDataStream >> uav.time;
		inputDataStream >> uav.state;
		inputDataStream >> uav.latLonStdDev;
		inputDataStream >> uav.latLon;
		inputDataStream >> uav.speed;
		inputDataStream >> uav.altitude;
		inputDataStream >> uav.course;
		inputDataStream >> uav.frequency;
		dataStream << uav;
	}

	return dataToSend;
}
