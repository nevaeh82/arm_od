#include "RpcClientRWrapper.h"

RpcClientRWrapper::RpcClientRWrapper(QObject *parent) :
	QObject(parent)
{
	connect(this, SIGNAL(initSignal()), this, SLOT(initSlot()));
	connect(this, SIGNAL(sendDataSignal(QSharedPointer<IMessageOld>))
			, this, SLOT(sendDataSlot(QSharedPointer<IMessageOld>)));
}

void RpcClientRWrapper::init(quint16 port, QHostAddress& address, IRouter* router)
{
	m_address = address;
	m_port = port;
	m_router = router;

	emit initSignal();
}

void RpcClientRWrapper::setId(int id)
{
	m_rpcClient->setId(id);
}

int RpcClientRWrapper::getId()
{
	return m_rpcClient->getId();
}

void RpcClientRWrapper::setType(int type)
{
	m_rpcClient->setType(type);
}

int RpcClientRWrapper::getType()
{
	return m_rpcClient->getType();
}

void RpcClientRWrapper::sendData(QSharedPointer<IMessageOld> msg_ptr)
{
	emit sendDataSignal(msg_ptr);
}

void RpcClientRWrapper::initSlot()
{
	m_rpcClient = new RPCClient_R(m_router, this);
	m_rpcClient->start(m_port, m_address);
}

void RpcClientRWrapper::sendDataSlot(QSharedPointer<IMessageOld> msg_ptr)
{
	m_rpcClient->sendData(msg_ptr);
}
