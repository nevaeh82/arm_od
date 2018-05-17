#include "RpcClientBase.h"
#include "Rpc/QxtRpcPeerWrapper.h"

RpcClientBase::RpcClientBase(QObject* parent) :
	QObject(parent)
{
    qRegisterMetaType<QAbstractSocket::SocketError>();
    m_clientPeer = new QxtRpcPeerWrapper;

    reconnectTimer = new QTimer;
	connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(connectToServer()));

	connect( this, SIGNAL(methodCalled(QString,QVariant,IRpcListener*)),
			 SLOT(internalCall(QString,QVariant,IRpcListener*)) );
}

RpcClientBase::~RpcClientBase()
{
    m_clientPeer->deleteLater();
}

bool RpcClientBase::start(quint16 port, QHostAddress address)
{
	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SLOT(connectedToServerSlot()) );
	connect(m_clientPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(connectionToServerFailedSlot()));
	connect(m_clientPeer, SIGNAL(disconnectedFromServer()), this, SLOT(connectionToServerFailedSlot()));

	m_port = port;
	m_address = address;

	connectToServer();

	return true;
}

void RpcClientBase::stop()
{
	m_clientPeer->disconnectAll();
}

void RpcClientBase::call(const QString& method, const QVariant& argument, IRpcListener* sender)
{
	emit methodCalled( method, argument, sender );
}

void RpcClientBase::internalCall(const QString& method, const QVariant& argument, IRpcListener* sender)
{
	Q_UNUSED( sender );
	m_clientPeer->call( method, argument );
}

void RpcClientBase::connectToServer()
{
	log_debug(QString("RPC-client status: connecting to server %1:%2").arg(m_address.toString()).arg(QString::number(m_port)));
	m_clientPeer->connect(m_address, m_port);
	log_debug("2");
}

void RpcClientBase::connectedToServerSlot()
{
	log_info(QString("RPC-client status: connected to server %1:%2").arg(m_address.toString()).arg(QString::number(m_port)));
	reconnectTimer->stop();

	emit onBaseConnected();
	emit signalARMRConnect(true);
}

void RpcClientBase::connectionToServerFailedSlot()
{
	log_warning(QString("ERROR RPC-client unable to connect to server %1:%2").arg(m_address.toString()).arg(QString::number(m_port)));
	reconnectTimer->start(1000);

	emit onBaseDisconnected();
	emit signalARMRConnect(false);
}
