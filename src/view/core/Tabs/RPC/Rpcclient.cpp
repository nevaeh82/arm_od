#include "RPCClient.h"
#include "Tabs/DbBla/Defines.h"

RPCClient::RPCClient(QObject* parent) :
	RpcClientBase(parent)
{
	connect(this, SIGNAL(startInternalSignal(quint16, QString)), this, SLOT(startInternalSlot(quint16, QString)));
	m_station = NULL;
	m_solverAuto = true;
}

void RPCClient::setStation(Station* station)
{
	m_station = station;
}

RPCClient::~RPCClient()
{
	log_debug("~RPCClient");
	m_clientPeer->disconnectAll();
}

bool RPCClient::start(quint16 port, QHostAddress ipAddress)
{
	emit startInternalSignal(port, ipAddress.toString());

	return true;
}

void RPCClient::setCommand(IMessageOld *msg)
{
	emit signalSetCommand(msg);
}

void RPCClient::slotSetCommand(IMessageOld *msg)
{
	m_commandMsg = msg;
	formCommand(m_commandMsg);
}

void RPCClient::startInternalSlot(quint16 port, QString ipAddress)
{
	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SLOT(slotRCPConnetion()));
	connect(m_clientPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(_slotErrorRPCConnection(QAbstractSocket::SocketError)));

	connect(this, SIGNAL(signalSetCommand(IMessageOld*)), this, SLOT(slotSetCommand(IMessageOld*)));

	m_clientPeer->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetNIIPPBPLA(QByteArray)), RPC_SLOT_SET_NIIPP_BPLA);
	m_clientPeer->attachSignal(this, SIGNAL(signalGetNIIPPStatus(QByteArray)), RPC_SLOT_GET_NIIPP_CONNECTION_STATUS);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetSolverData(QByteArray)), RPC_SLOT_SET_SOLVER_DATA);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetSolverDataClear(QByteArray)), RPC_SLOT_SET_SOLVER_CLEAR);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetAdsb(QByteArray)), RPC_SLOT_SET_ADSB_ENABLE);

	connect(this, SIGNAL(signalReconnection()), this, SLOT(_slotReconnection()));
	connect(m_clientPeer, SIGNAL(disconnectedFromServer()), this, SLOT(_slotRPCDisconnection()));

	///server
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BLA_POINTS, this, SLOT(rpcSendBlaPoints(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_AIS_DATA, this, SLOT(rpcSlotServerSendAisData(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_POINTS, this, SLOT(rpcSendBplaPoints(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_POINTS_1, this, SLOT(rpcSendBplaPoints1(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO, this, SLOT(rpcSendBplaPointsAuto(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_POINTS_SINGLE, this, SLOT(rpcSendBplaPointsSingle(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_HYPERBOLA, this, SLOT(rpcSendHyperbola(QByteArray)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_NIIPP_DATA, this, SLOT(rpcSendNiippData(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_NIIPP_CONNECTION_STATUS, this, SLOT(rpcSendNiippConnectionStatus(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION, this, SLOT(rpcSlotServerSendAtlantDirection(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_ATLANT_POSITION, this, SLOT(rpcSlotServerSendAtlantPosition(QByteArray)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_ADSB_DATA, this, SLOT(rpcSlotServerSendAdbsData(QByteArray)));

	log_debug("Start RPCClient");
	RpcClientBase::start(port, QHostAddress(ipAddress));
}

void RPCClient::formCommand(IMessageOld *msg)
{
	qDebug() << "form command";
	QByteArray data;
	int type = msg->get(data);
	switch(type)
	{
	case COMMAND_SET_NIIPP_BPLA:
		sendNiippBpla(data);
		break;
	case COMMAND_GET_NIIPP_CONNECTION_STATUS:
		sendNiippConnectionStatus(data);
		break;
	case COMMAND_SET_SOLVER:
		sendDataToSovler(data);
		break;
	case COMMAND_SET_SOLVER_AUTO:
		setSolverAuto(data);
		break;

	case COMMAND_SET_SOLVER_CLEAR:
		setSolverClear(data);
		break;
	case COMMAND_SET_ADSB:
		setAdsb(data);
		break;

	default:
		break;
	}
	msg->clenup();
}

void RPCClient::setSolverAuto(QByteArray ba)
{
	QDataStream ds(&ba, QIODevice::ReadOnly);
	ds >> m_solverAuto;
}

/// slot when connection complete
void RPCClient::slotRCPConnetion()
{
	qDebug() << "Connection complete!";
	emit signalSetClientId(m_station->id);
}

void RPCClient::rpcSendBlaPoints(QByteArray data)
{
	foreach (IRpcListener* reciever, m_receiversList) {
		reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_BLA_POINTS, QVariant(data));
	}
}

void RPCClient::rpcSlotServerSendAisData(QByteArray data)
{
	foreach (IRpcListener* reciever, m_receiversList) {
		reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_AIS_DATA, QVariant(data));
	}
}

void RPCClient::rpcSendBplaPoints(QByteArray data)
{
	//DISABLE auto mode for receiving points
	//if(m_solverAuto == false) {
//		foreach (IRpcListener* reciever, m_receiversList) {
//			reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_BPLA_POINTS, QVariant(data));
//		}
	//}
}

void RPCClient::rpcSendBplaPoints1(QByteArray data)
{
	//DISABLE auto mode for receiving points
	//if(m_solverAuto == false) {
		foreach (IRpcListener* reciever, m_receiversList) {
			reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_BPLA_POINTS_1, QVariant(data));
		}
	//}
}

void RPCClient::rpcSendBplaPointsAuto(QByteArray data)
{
	//DISABLE auto mode for receiving points
	//if(m_solverAuto == true) {
		foreach (IRpcListener* reciever, m_receiversList) {
			reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO, QVariant(data));
		}
	//}
}

void RPCClient::rpcSendBplaPointsSingle(QByteArray data)
{
	//DISABLE auto mode for receiving points
	//if(m_solverAuto == true) {
	foreach (IRpcListener* reciever, m_receiversList) {
		reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_BPLA_POINTS_SINGLE, QVariant(data));
	}
	//}
}

void RPCClient::rpcSendHyperbola(QByteArray data)
{
	foreach (IRpcListener* reciever, m_receiversList) {
		reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_HYPERBOLA, QVariant(data));
	}
}

void RPCClient::rpcSendNiippData(QByteArray data)
{
	foreach (IRpcListener* reciever, m_receiversList) {
		reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_NIIPP_DATA, QVariant(data));
	}
}

void RPCClient::rpcSendNiippConnectionStatus(QByteArray data)
{
	foreach (IRpcListener* reciever, m_receiversList) {
		reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_NIIPP_CONNECTION_STATUS, QVariant(data));
	}
}



void RPCClient::rpcSlotServerSendAtlantDirection(QByteArray data)
{
	foreach (IRpcListener* reciever, m_receiversList) {
		reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION, QVariant(data));
	}
}

void RPCClient::rpcSlotServerSendAtlantPosition(QByteArray data)
{
	foreach (IRpcListener* reciever, m_receiversList) {
		reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_ATLANT_POSITION, QVariant(data));
	}
}

void RPCClient::rpcSlotServerSendAdbsData(QByteArray data)
{
	foreach (IRpcListener* reciever, m_receiversList) {
		reciever->onMethodCalled(RPC_SLOT_SERVER_SEND_ADSB_DATA, QVariant(data));
	}
}

void RPCClient::sendNiippBpla(QByteArray data)
{
	emit signalSetNIIPPBPLA(data);
}

void RPCClient::sendNiippConnectionStatus(QByteArray data)
{
	emit signalGetNIIPPStatus(data);
}

void RPCClient::setSolverClear(QByteArray data)
{
	emit signalSetSolverDataClear(data);
}

void RPCClient::setAdsb(QByteArray data)
{
	emit signalSetAdsb( data );
}

void RPCClient::sendDataToSovler(QByteArray data)
{
	emit signalSetSolverData(data);
}
