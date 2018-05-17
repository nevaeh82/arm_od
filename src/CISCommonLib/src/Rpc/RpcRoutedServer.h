#ifndef RPCROUTEDSERVER_H
#define RPCROUTEDSERVER_H

#include "Templates/BiMap.h"
#include "Rpc/RpcServerBase.h"

// default command to register route for RPC client
#define  RPC_METHOD_REGISTER_CLIENT		"RpcRoutedServer::registerClient"
#define  RPC_METHOD_DEREGISTER_CLIENT	"RpcRoutedServer::deregisterClient"

class RpcRoutedServer : public RpcServerBase
{
	Q_OBJECT

	friend class TestRpcRoutedServer;

public:
	typedef quint64 ClientId;
	typedef uint RouteId;

private:

protected:
	// routes, that links listeners with RPC clients
	BiMap<ClientId, IRpcListener* > m_routes;

	// map of route ids and RPC client ids
	QMap<RouteId, ClientId > m_clientRoutes;

	// map of route ids and listeners
	QMap<RouteId, IRpcListener* > m_listenerRoutes;

public:
	explicit RpcRoutedServer(QObject* parent = NULL);
	virtual ~RpcRoutedServer(){  }

	/**
	 * Call listener handler linked with RPC client, or handlers of all listeners, if client = 0
	 */
	virtual void dispatch(const QString& method, const QVariant& argument, const ClientId& id = 0);

	/**
	 * Register receiver for messages from RPC clients with route id
	 *
	 * @return false, if route id or listener already registered
	 */
	virtual bool registerReceiver(IRpcListener* receiver, const RouteId& routeId = 0);

	/**
	 * Deregister receiver for messages from RPC clients
	 */
	virtual void deregisterReceiver(IRpcListener* receiver);

protected slots:
	/**
	 * Call method on client linked with sender, or on all clients, if sender is NULL
	 */
	virtual void internalCall(const QString& method, const QVariant& argument, IRpcListener *sender = NULL);

	/**
	 * Register RPC client for messages from listeners with route id
	 *
	 * @return false, if route id or client already registered
	 */
	virtual bool registerClient(const quint64& clientId, const uint& routeId);

	/**
	 * Deregister RPC client for messages from listeners
	 */
	virtual void deregisterClient(const quint64& clientId);

signals:
	void onConnected();
};

#endif // RPCROUTEDSERVER_H
