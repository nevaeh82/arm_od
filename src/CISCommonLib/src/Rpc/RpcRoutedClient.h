#ifndef RPCROUTEDCLIENT_H
#define RPCROUTEDCLIENT_H

#include "Rpc/RpcClientBase.h"
#include "Rpc/RpcRoutedServer.h"

class RpcRoutedClient : public RpcClientBase
{
	Q_OBJECT

	friend class TestRpcRoutedClient;

protected:
	RpcRoutedServer::RouteId m_routeId;

	const QString m_registerMethod;
	const QString m_deregisterMethod;

public:
	explicit RpcRoutedClient(
			const QString& registerMethod = RPC_METHOD_REGISTER_CLIENT,
			const QString& deregisterMethod = RPC_METHOD_DEREGISTER_CLIENT,
			QObject *parent = 0);

	/**
	 * Send register route command to RPC server
	 *
	 * @return if client not connected, return false
	 */
	virtual bool registerRoute(RpcRoutedServer::RouteId id);

	/**
	 * Send deregister route command to RPC server
	 */
	virtual void deregisterRoute();
};

#endif // RPCROUTEDCLIENT_H
