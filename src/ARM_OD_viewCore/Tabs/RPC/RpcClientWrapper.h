#ifndef RPCCLIENTWRAPPER_H
#define RPCCLIENTWRAPPER_H

#include <QObject>
#include "RPCClient.h"

class RpcClientWrapper : public QObject
{
	Q_OBJECT
private:
	quint16 m_port;
	QHostAddress m_address;
	RPCClient* m_rpcClient;

	Station* m_station;
	/*
	IDbUavManager* m_dbManager;
	IMapController* m_mapController;
	ITabMap* m_parentTab;
	ITabManager* m_tabManager;
	*/

public:
	explicit RpcClientWrapper(QObject *parent = 0);
	virtual ~RpcClientWrapper();

	void registerReceiver(IRpcListener* receiver);
	void deregisterReceiver(IRpcListener* receiver);

	void init(quint16, QHostAddress&, Station*/*, IDbUavManager*, IMapController*, ITabMap*, ITabManager**/);
	void setCommand(IMessageOld* msg);

	void stop();

signals:
	void initSignal();
	void stopSignal();
	void signalSetCommand(IMessageOld*);

	void initFinishedSignal();

private slots:
	void initSlot();
	void stopSlot();
	void slotSetCommand(IMessageOld* msg);
	
};

#endif // RPCCLIENTWRAPPER_H
