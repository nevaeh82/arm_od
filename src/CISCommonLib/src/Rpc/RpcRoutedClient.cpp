#include "RpcRoutedClient.h"

RpcRoutedClient::RpcRoutedClient(
		const QString& registerMethod,
		const QString& deregisterMethod,
		QObject *parent)
	: RpcClientBase( parent )
	, m_routeId( 0 )
	, m_registerMethod( registerMethod )
	, m_deregisterMethod( deregisterMethod )
{
}

bool RpcRoutedClient::registerRoute(RpcRoutedServer::RouteId id)
{
	if (id == 0) return false;
	if (!m_clientPeer->isClient()) return false;

	// deregister first, if already registered
	if (m_routeId != 0) {
		deregisterRoute();
	}

	Q_ASSERT( !m_registerMethod.isEmpty() );

	m_routeId = id;
	m_clientPeer->call( m_registerMethod, QVariant(m_routeId) );

	return true;
}

void RpcRoutedClient::deregisterRoute()
{
	if (m_routeId == 0) return;
	if (!m_clientPeer->isClient()) return;

	Q_ASSERT( !m_deregisterMethod.isEmpty() );

	m_routeId = 0;
	m_clientPeer->call( m_deregisterMethod );
}
