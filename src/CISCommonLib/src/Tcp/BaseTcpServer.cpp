#include "BaseTcpServer.h"

BaseTcpServer::BaseTcpServer(QObject *parent) :
	QObject(parent)
{
	m_tcpServer = new QTcpServer(this);
	m_clientsCount = 0;

	m_host = QHostAddress::Any;
	m_port = 6662;

	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnectionSlot()));
}

void BaseTcpServer::start(const QHostAddress& host, const quint16 port)
{
	m_host = host;
	m_port = port;

	m_tcpServer->listen(m_host, m_port);
}

void BaseTcpServer::stop()
{
	m_tcpServer->close();
}

bool BaseTcpServer::isListening()
{
	return m_tcpServer->isListening();
}

bool BaseTcpServer::sendData(const QByteArray &data)
{
	if (m_clients.count() == 0){
		return false;
	}

	foreach (uint clientId, m_clients.keys()){
		if (!sendData(clientId, data)){
			return false;
		}
	}

	return true;
}

bool BaseTcpServer::sendData(const uint clientId, const QByteArray &data)
{
	if (!m_clients.contains(clientId)){
		return false;
	}

	ITcpServerClient* client = m_clients.value(clientId);

	client->writeData(data);

	return true;
}

void BaseTcpServer::onDataReceived(const QVariant &argument)
{
	foreach (ITcpReceiver* receiver, m_receiversList) {
		receiver->onDataReceived(argument);
	}
}

ITcpServerClient* BaseTcpServer::getClientById(uint clientId)
{
	if (!m_clients.contains(clientId)){
		return NULL;
	}

	return m_clients.value(clientId);
}

void BaseTcpServer::onNewConnectionSlot()
{
	QTcpSocket* tcpSocket = m_tcpServer->nextPendingConnection();
	BaseTcpServerClient* client = new BaseTcpServerClient(tcpSocket, this);
	client->registerReceiver(this);

	emit newClientSignal(m_clientsCount, client);

	m_clients.insert(m_clientsCount, client);
	m_clientsCount++;
}
