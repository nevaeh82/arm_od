#include "Rpc/RpcRoutedServer.h"

RpcRoutedServer::RpcRoutedServer(QObject* parent)
	: RpcServerBase( parent )
{
	connect( m_serverPeer, SIGNAL(clientDisconnected(quint64)), SLOT(deregisterClient(quint64)) );
}

void RpcRoutedServer::internalCall(const QString& method, const QVariant& argument, IRpcListener* sender)
{
	// if sender not specified or not registered or client for sender not registered
	if( sender == NULL ) {
		RpcServerBase::internalCall( method, argument, sender );
		return;
	}

	if( !m_routes.containsRight( sender ) ) return;

	ClientId id = m_routes.left( sender );
	m_serverPeer->call( id, method, argument );
}

void RpcRoutedServer::dispatch(
		const QString& method,
		const QVariant& argument,
		const RpcRoutedServer::ClientId& id)
{
	if (id == 0) {
		foreach (IRpcListener *receiver, m_receiversList) {
			receiver->onMethodCalled( method, argument );
		}

		return;
	}

	if (!m_routes.containsLeft( id )) return;

	m_routes.right( id )->onMethodCalled( method, argument );
}

bool RpcRoutedServer::registerReceiver(IRpcListener* receiver, const RouteId& routeId)
{
	if ( receiver == NULL ) return false;

	if ( m_listenerRoutes.contains( routeId ) ) return false;
	if ( m_listenerRoutes.values().contains( receiver ) ) return false;
	if ( m_routes.containsRight( receiver ) ) return false;

	RpcServerBase::registerReceiver( receiver );

	// if route is null, return true, because receiver
	// registered for messages from all clients
	if ( routeId == 0 ) return true;

	m_listenerRoutes.insert( routeId, receiver );

	// if client routes contains client for inserting route,
	// insert directly into the routes table
	if (m_clientRoutes.contains( routeId )) {
		ClientId clientId = m_clientRoutes.value( routeId );

		// if routes already contains gotted client, we do nothing
		if ( !m_routes.containsLeft( clientId ) ) {
			m_routes.insert( clientId, receiver );
		}
	}

	return true;
}

void RpcRoutedServer::deregisterReceiver(IRpcListener* receiver)
{
	RpcServerBase::deregisterReceiver( receiver );

	if ( receiver == NULL ) return;

	if ( m_routes.containsRight( receiver ) ) {
		m_routes.removeByRight( receiver );
	}

	if ( m_listenerRoutes.values().contains( receiver ) ) {
		m_listenerRoutes.remove( m_listenerRoutes.key( receiver ) );
	}
}

bool RpcRoutedServer::registerClient(const quint64& clientId, const uint& routeId)
{
	if ( clientId == 0 ) return false;
	if ( m_clientRoutes.contains( routeId ) ) return false;
	if ( m_clientRoutes.values().contains( clientId ) ) return false;
	if ( m_routes.containsLeft( clientId ) ) return false;

	m_clientRoutes.insert( routeId, clientId );

	// if listener routes contains listener for inserting route,
	// insert directly into the routes table
	if (m_listenerRoutes.contains( routeId )) {
		IRpcListener* listener = m_listenerRoutes.value( routeId );

		// if routes already contains gotted listener, we do nothing
		if ( !m_routes.containsRight( listener ) ) {
			m_routes.insert( clientId, listener );
		}
	}

	emit onConnected();

	return true;
}

void RpcRoutedServer::deregisterClient(const quint64& clientId)
{
	if ( clientId == 0 ) return;

	if ( m_routes.containsLeft( clientId ) ) {
		m_routes.removeByLeft( clientId );
	}

	if ( m_clientRoutes.values().contains( clientId ) ) {
		m_clientRoutes.remove( m_clientRoutes.key( clientId ) );
	}
}
