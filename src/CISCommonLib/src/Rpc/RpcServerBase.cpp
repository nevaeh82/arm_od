#include "RpcServerBase.h"
#include "Rpc/QxtRpcPeerWrapper.h"

RpcServerBase::RpcServerBase(QObject* parent) :
	QObject(parent)
{
    m_serverPeer = new QxtRpcPeerWrapper;

	connect( this, SIGNAL(methodCalled(QString,QVariant,IRpcListener*)),
			 SLOT(internalCall(QString,QVariant,IRpcListener*)) );
}

bool RpcServerBase::start(quint16 port, QHostAddress address)
{
	bool result = m_serverPeer->listen(address, port);

	if (!result){
		log_warning(QString("RPC-server was not started on %1:%2").arg(address.toString()).arg(QString::number(port)));
		return false;
	}

	log_info(QString("RPC-server started successfully and listens to %1:%2").arg(address.toString()).arg(QString::number(port)));

	return true;
}

void RpcServerBase::stop()
{
	m_serverPeer->stopListening();
	m_serverPeer->disconnectAll();
}

void RpcServerBase::call(const QString& method, const QVariant& argument, IRpcListener* sender)
{
	emit methodCalled( method, argument, sender );
}

void RpcServerBase::internalCall(const QString& method, const QVariant& argument, IRpcListener* sender)
{
	Q_UNUSED( sender );
	m_serverPeer->call( method, argument );
}
