#include "RpcClientWrapper.h"

RpcClientWrapper::RpcClientWrapper(QObject *parent) :
	QObject(parent)
{
	m_rpcClient = NULL;
}

RpcClientWrapper::~RpcClientWrapper()
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return;
	}
	m_rpcClient->stop();
	//m_rpcClient->deleteLater();
}

void RpcClientWrapper::registerReceiver(IRpcListener* receiver)
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return;
	}
	m_rpcClient->registerReceiver(receiver);
}

void RpcClientWrapper::deregisterReceiver(IRpcListener* receiver)
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return;
	}
	m_rpcClient->deregisterReceiver(receiver);
}

void RpcClientWrapper::init(quint16 port, QHostAddress& address, Station* station)
{
	m_address = address;
	m_port = port;

	m_station = station;

	connect(this, SIGNAL(initSignal()), this, SLOT(initSlot()));
	connect(this, SIGNAL(signalSetCommand(IMessageOld*)), this, SLOT(slotSetCommand(IMessageOld*)));

	emit initSignal();
}

void RpcClientWrapper::setCommand(IMessageOld* msg)
{
	emit signalSetCommand(msg);
}

void RpcClientWrapper::stop()
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return;
	}
	m_rpcClient->stop();
}

void RpcClientWrapper::slotSetCommand(IMessageOld* msg)
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return;
	}
	m_rpcClient->setCommand(msg);
}

void RpcClientWrapper::initSlot()
{
	m_rpcClient = new RPCClient(this);
	m_rpcClient->setStation(m_station);
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return;
	}
	m_rpcClient->start(m_port, m_address);

	emit initFinishedSignal();
}

void RpcClientWrapper::stopSlot()
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return;
	}
	m_rpcClient->stop();
}
