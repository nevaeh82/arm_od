#include "RpcClientRWrapper.h"

RpcClientRWrapper::RpcClientRWrapper(QObject *parent) :
	QObject(parent)
{
	m_rpcClient = NULL;
}

void RpcClientRWrapper::init(quint16 port, QHostAddress& address, IRouter* router)
{
	m_address = address;
	m_port = port;
	m_router = router;

	connect(this, SIGNAL(initSignal()), this, SLOT(initSlot()));
	connect(this, SIGNAL(sendDataSignal(QSharedPointer<IMessageOld>))
			, this, SLOT(sendDataSlot(QSharedPointer<IMessageOld>)));

	connect(this, SIGNAL(sendDataByRpcSignal(QString,QByteArray)), this, SLOT(sendDataByRpcSlot(QString,QByteArray)));

	emit initSignal();
}

void RpcClientRWrapper::setId(int id)
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return;
	}
	m_rpcClient->setId(id);
}

int RpcClientRWrapper::getId()
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return -1000;
	}
	return m_rpcClient->getId();
}

void RpcClientRWrapper::setType(int type)
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return;
	}
	m_rpcClient->setType(type);
}

int RpcClientRWrapper::getType()
{
	if (m_rpcClient == NULL) {
		log_debug("m_rpcClient == NULL");
		return -1000;
	}
	return m_rpcClient->getType();
}

void RpcClientRWrapper::sendData(QSharedPointer<IMessageOld> msg_ptr)
{
	m_rpcClient->sendData(msg_ptr);
	//emit sendDataSignal(msg_ptr);
}

void RpcClientRWrapper::sendDataByRpc(const QString& signalType, const QByteArray& data)
{
	emit sendDataByRpcSignal(signalType, data);
}

void RpcClientRWrapper::initSlot()
{
	m_rpcClient = new RPCClient_R(m_router, this);
	m_rpcClient->start(m_port, m_address);
}

void RpcClientRWrapper::sendDataByRpcSlot(QString signalType, QByteArray data)
{
	m_rpcClient->sendDataByRpc(signalType, data);
}
