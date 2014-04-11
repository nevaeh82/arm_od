#include "RpcClientWrapper.h"

RpcClientWrapper::RpcClientWrapper(QObject *parent) :
	QObject(parent)
{
	connect(this, SIGNAL(initSignal()), this, SLOT(initSlot()));
	connect(this, SIGNAL(signalSetCommand(IMessageOld*)), this, SLOT(slotSetCommand(IMessageOld*)));
}

void RpcClientWrapper::init(quint16 port, QHostAddress& address,
							Station* station, IDbUavManager* uavDbManager,
							IMapController* mapController, ITabMap* tabMap, ITabManager* tabManager)
{
	m_address = address;
	m_port = port;

	m_station = station;
	m_dbManager = uavDbManager;
	m_mapController = mapController;
	m_parentTab = tabMap;
	m_tabManager = tabManager;

	emit initSignal();
}

void RpcClientWrapper::setCommand(IMessageOld* msg)
{
	emit signalSetCommand(msg);
}

void RpcClientWrapper::slotSetCommand(IMessageOld* msg)
{
	m_rpcClient->setCommand(msg);
}

void RpcClientWrapper::initSlot()
{
	m_rpcClient = new RPCClient(m_station, m_dbManager, m_mapController, m_parentTab, m_tabManager, this);
	m_rpcClient->start(m_port, m_address);
}