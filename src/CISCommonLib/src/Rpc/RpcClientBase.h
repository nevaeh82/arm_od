#ifndef RPCCLIENTBASE_H
#define RPCCLIENTBASE_H

#include <QObject>
#include <QTimer>
#include <QHostAddress>
#include <QScopedPointer>

#include "Logger/Logger.h"
#include "Rpc/RpcPeerAbstract.h"
#include "Interfaces/IRpcControllerBase.h"

Q_DECLARE_METATYPE(QAbstractSocket::SocketError)

class RpcClientBase: public QObject, public IRpcControllerBase
{
	Q_OBJECT

private:
	quint16 m_port;
	QHostAddress m_address;
	QTimer* reconnectTimer;

protected:
	RpcPeerAbstract* m_clientPeer;

public:
	explicit RpcClientBase(QObject* parent = NULL);
    virtual ~RpcClientBase();

	virtual bool start(quint16 port, QHostAddress address);
	virtual void stop();

	virtual void call(const QString& method, const QVariant& argument, IRpcListener *sender = NULL);

signals:
	void methodCalled(const QString& method, const QVariant& argument, IRpcListener *sender = NULL);
	void signalARMRConnect(bool);

	void onBaseDisconnected();
	void onBaseConnected();

protected slots:
	virtual void internalCall(const QString& method, const QVariant& argument, IRpcListener *sender = NULL);

private slots:
	void connectToServer();
	void connectedToServerSlot();
	void connectionToServerFailedSlot();
};

#endif // RPCCLIENTBASE_H
