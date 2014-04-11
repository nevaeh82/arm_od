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
	IDbUavManager* m_dbManager;
	IMapController* m_mapController;
	ITabMap* m_parentTab;
	ITabManager* m_tabManager;

public:
	explicit RpcClientWrapper(QObject *parent = 0);
	~RpcClientWrapper();
	void init(quint16, QHostAddress&, Station*, IDbUavManager*, IMapController*, ITabMap*, ITabManager*);
	void setCommand(IMessageOld* msg);

signals:
	void initSignal();
	void signalSetCommand(IMessageOld*);

private slots:
	void initSlot();
	void slotSetCommand(IMessageOld* msg);
	
};

#endif // RPCCLIENTWRAPPER_H
