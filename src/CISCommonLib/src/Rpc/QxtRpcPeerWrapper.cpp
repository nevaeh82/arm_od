#include "QxtRpcPeerWrapper.h"

#include "Logger/Logger.h"

QxtRpcPeerWrapper::QxtRpcPeerWrapper(QObject *parent)
	: RpcPeerAbstract(parent)
	, m_peer(new QxtRPCPeer(parent))
{
	QObject::connect(m_peer, SIGNAL(connectedToServer()), this, SIGNAL(connectedToServer()));
	QObject::connect(m_peer, SIGNAL(destroyed()), this, SIGNAL(destroyed()));
	QObject::connect(m_peer, SIGNAL(destroyed(QObject*)), this, SIGNAL(destroyed(QObject*)));
	QObject::connect(m_peer, SIGNAL(clientConnected(quint64)),
					 this, SIGNAL(clientConnected(quint64)));
	QObject::connect(m_peer, SIGNAL(clientDisconnected(quint64)),
					 this, SIGNAL(clientDisconnected(quint64)));
	QObject::connect(m_peer, SIGNAL(disconnectedFromServer()),
					 this, SIGNAL(disconnectedFromServer()));
	QObject::connect(m_peer, SIGNAL(serverError(QAbstractSocket::SocketError)),
					 this, SIGNAL(serverError(QAbstractSocket::SocketError)));
}

QxtRpcPeerWrapper::~QxtRpcPeerWrapper()
{
	delete m_peer;
}

bool QxtRpcPeerWrapper::isServer() const
{
	return m_peer->isServer();
}

bool QxtRpcPeerWrapper::isClient() const
{
	return m_peer->isClient();
}

void QxtRpcPeerWrapper::connect(QHostAddress addr, int port)
{
	m_peer->connect(addr, port);
}

void QxtRpcPeerWrapper::connect(QString addr, int port)
{
	m_peer->connect(addr, port);
}

bool QxtRpcPeerWrapper::listen(QHostAddress iface, int port)
{
	return m_peer->listen(iface, port);
}
void QxtRpcPeerWrapper::stopListening()
{
	m_peer->stopListening();
}

QList<quint64> QxtRpcPeerWrapper::clients() const
{
	return m_peer->clients();
}

QIODevice* QxtRpcPeerWrapper::device() const
{
	return m_peer->device();
}

void QxtRpcPeerWrapper::setDevice(QIODevice* dev)
{
	m_peer->setDevice(dev);
}

QIODevice* QxtRpcPeerWrapper::takeDevice()
{
	return m_peer->takeDevice();
}

QxtAbstractSignalSerializer* QxtRpcPeerWrapper::serializer() const
{
	return m_peer->serializer();
}

void QxtRpcPeerWrapper::setSerializer(QxtAbstractSignalSerializer* serializer)
{
	m_peer->setSerializer(serializer);
}

QxtAbstractConnectionManager* QxtRpcPeerWrapper::connectionManager() const
{
	return m_peer->connectionManager();
}

void QxtRpcPeerWrapper::setConnectionManager(QxtAbstractConnectionManager* manager)
{
	m_peer->setConnectionManager(manager);
}

bool QxtRpcPeerWrapper::attachSignal(QObject* sender, const char* signal,
									 const QString& rpcFunction)
{
	return m_peer->attachSignal(sender, signal, rpcFunction);
}

bool QxtRpcPeerWrapper::attachSlot(const QString& rpcFunction, QObject* recv, const char* slot,
				Qt::ConnectionType type)
{
	return m_peer->attachSlot(rpcFunction, recv, slot, type);
}

void QxtRpcPeerWrapper::detachSignals(QObject* obj)
{
	m_peer->detachSignals(obj);
}
void QxtRpcPeerWrapper::detachSlots(QObject* obj)
{
	m_peer->detachSlots(obj);
}
void QxtRpcPeerWrapper::detachObject(QObject* obj)
{
	m_peer->detachObject(obj);
}

void QxtRpcPeerWrapper::disconnectClient(quint64 id)
{
	m_peer->disconnectClient(id);
}
void QxtRpcPeerWrapper::disconnectServer()
{
	m_peer->disconnectServer();
}
void QxtRpcPeerWrapper::disconnectAll()
{
	m_peer->disconnectAll();
}

void QxtRpcPeerWrapper::call(QString fn,
		  const QVariant& p1, const QVariant& p2, const QVariant& p3, const QVariant& p4,
		  const QVariant& p5, const QVariant& p6, const QVariant& p7, const QVariant& p8)
{
	m_peer->call(fn, p1, p2, p3, p4, p5, p6, p7, p8);
}

void QxtRpcPeerWrapper::call(QList<quint64> ids, QString fn,
		  const QVariant& p1, const QVariant& p2, const QVariant& p3, const QVariant& p4,
		  const QVariant& p5, const QVariant& p6, const QVariant& p7, const QVariant& p8)
{
	m_peer->call(ids, fn, p1, p2, p3, p4, p5, p6, p7, p8);
}

void QxtRpcPeerWrapper::call(quint64 id, QString fn,
		  const QVariant& p1, const QVariant& p2, const QVariant& p3, const QVariant& p4,
		  const QVariant& p5, const QVariant& p6, const QVariant& p7, const QVariant& p8)
{
	m_peer->call(id, fn, p1, p2, p3, p4, p5, p6, p7, p8);
}

void QxtRpcPeerWrapper::callExcept(quint64 id, QString fn,
				const QVariant& p1, const QVariant& p2, const QVariant& p3, const QVariant& p4,
				const QVariant& p5, const QVariant& p6, const QVariant& p7, const QVariant& p8)
{
	m_peer->callExcept(id, fn, p1, p2, p3, p4, p5, p6, p7, p8);
}

void QxtRpcPeerWrapper::detachSender()
{
	m_peer->detachSender();
}
