#ifndef RPCCONFIGCLIENT_H
#define RPCCONFIGCLIENT_H

#include <Rpc/RpcClientBase.h>

#include "RPC/RpcDefines.h"
#include "Interfaces/IRpcConfigClient.h"

class RpcConfigClient : public RpcClientBase, public IRpcConfigClient
{
	Q_OBJECT

public:
	explicit RpcConfigClient(QObject* parent = NULL);
	virtual ~RpcConfigClient();

	virtual inline bool isConnected() { return m_clientPeer->isClient(); }

	// IRpcConfigClient interface
public:
	virtual void requestGetStationList(const QString& filename);
	virtual void requestGetMapObjects(const QString& filename);
	virtual void requestGetMapPoints(const QString& filename);
	virtual void requestGetDbConfiguration(const QString& filename);

	// IRpcControllerBase interface
public:
	virtual bool start(quint16 port, QHostAddress ipAddress);

signals:
	void getStationListSignal(QString);
	void getMapObjectsSignal(QString);
	void getMapPointsSignal(QString);
	void getDbConfigurationSignal(QString);

	void connectionEstablishedSignal();

private slots:
	void receivedStationListSlot(QByteArray data);
	void receivedMapObjectsSlot(QByteArray data);
	void receivedMapPointsSlot(QByteArray data);
	void receivedDbConfigurationSlot(QByteArray data);
};

#endif // RPCCONFIGCLIENT_H
