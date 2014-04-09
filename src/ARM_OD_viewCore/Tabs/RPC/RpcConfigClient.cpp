#include "RpcConfigClient.h"

RpcConfigClient::RpcConfigClient(QObject* parent) :
	RpcClientBase(parent)
{

}

RpcConfigClient::~RpcConfigClient()
{
}

void RpcConfigClient::receivedStationListSlot(QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, data);
	}
}

void RpcConfigClient::receivedMapObjectsSlot(QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_MAP_OBJECTS, data);
	}
}

void RpcConfigClient::receivedMapPointsSlot(QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_MAP_POINTS, data);
	}
}


void RpcConfigClient::receivedDbConfigurationSlot(QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION, data);
	}
}

void RpcConfigClient::requestGetStationList(const QString& filename)
{
//	m_clientPeer->call(RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST, filename);
	emit getStationListSignal(filename);
}

void RpcConfigClient::requestGetMapObjects(const QString& filename)
{
//	m_clientPeer->call(RPC_METHOD_CONFIG_REQUEST_GET_MAP_OBJECTS, filename);
	emit getMapObjectsSignal(filename);
}

void RpcConfigClient::requestGetMapPoints(const QString& filename)
{
//	m_clientPeer->call(RPC_METHOD_CONFIG_REQUEST_GET_MAP_POINTS, filename);
	emit getMapPointsSignal(fileName);
}

void RpcConfigClient::requestGetDbConfiguration(const QString& filename)
{
//	m_clientPeer->call(RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION, filename);
	emit getDbConfigurationSignal(filename);
}


bool RpcConfigClient::start(quint16 port, QHostAddress ipAddress)
{
	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SIGNAL(connectionEstablishedSignal()));

	m_clientPeer->attachSignal(this, SIGNAL(getStationListSignal(QString)), RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST);
	m_clientPeer->attachSignal(this, SIGNAL(getMapObjectsSignal(QString)), RPC_METHOD_CONFIG_REQUEST_GET_MAP_OBJECTS);
	m_clientPeer->attachSignal(this, SIGNAL(getMapPointsSignal(QString)), RPC_METHOD_CONFIG_REQUEST_GET_MAP_POINTS);
	m_clientPeer->attachSignal(this, SIGNAL(getDbConfigurationSignal(QString)), RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION);

	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, this, SLOT(receivedStationListSlot(QByteArray)));
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_MAP_OBJECTS, this, SLOT(receivedMapObjectsSlot(QByteArray)));
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_MAP_POINTS, this, SLOT(receivedMapPointsSlot(QByteArray)));
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION, this, SLOT(receivedDbConfigurationSlot(QByteArray)));

	log_debug("Start RpcConfigClient");
	RpcClientBase::start(port, ipAddress);
}
