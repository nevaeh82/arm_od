#ifndef RPCSERVERBASE_H
#define RPCSERVERBASE_H

#include <QObject>

#include "Logger/Logger.h"
#include "Rpc/RpcPeerAbstract.h"
#include "Interfaces/IRpcControllerBase.h"

class RpcServerBase : public QObject, public IRpcControllerBase
{
	Q_OBJECT

private:
	quint16 m_port;

protected:
	RpcPeerAbstract* m_serverPeer;

public:
	explicit RpcServerBase(QObject* parent = NULL);
	virtual ~RpcServerBase(){}

	virtual bool start(quint16 port, QHostAddress address = QHostAddress::Any);
	virtual void stop();

	virtual void call(const QString& method, const QVariant& argument, IRpcListener *sender = NULL);

signals:
	void methodCalled(const QString& method, const QVariant& argument, IRpcListener *sender = NULL);

protected slots:
	virtual void internalCall(const QString& method, const QVariant& argument, IRpcListener *sender = NULL);
};

#endif // RPCSERVERBASE_H
