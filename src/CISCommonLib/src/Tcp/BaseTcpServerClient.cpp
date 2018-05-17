#include "BaseTcpServerClient.h"

BaseTcpServerClient::BaseTcpServerClient(QTcpSocket* tcpSocket, QObject *parent) :
	QObject(parent)
{
	m_tcpSocket = tcpSocket;

	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
	connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketDisplayError(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(writeDataInternalSignal(QByteArray)), this, SLOT(writeDataInternalSlot(QByteArray)));
}

BaseTcpServerClient::~BaseTcpServerClient()
{
}

void BaseTcpServerClient::connectToHost(const QString&, const quint32)
{

}

void BaseTcpServerClient::disconnectFromHost()
{
	QMetaObject::invokeMethod( this, "disconnectFromHostInternalSlot", Qt::QueuedConnection );
	//emit disconnectFromHostInternalSignal();
}

bool BaseTcpServerClient::isConnected()
{
	return m_tcpSocket->state() == QAbstractSocket::ConnectedState;
}

void BaseTcpServerClient::writeData(const QByteArray &data)
{
	emit writeDataInternalSignal(data);
}

QString BaseTcpServerClient::getHost()
{
	QString host = m_tcpSocket->peerName() + ":" + QString::number(m_tcpSocket->peerPort());
	return host;
}

QObject *BaseTcpServerClient::asQObject()
{
	return this;
}

void BaseTcpServerClient::setReconnectInterval(const uint)
{

}

void BaseTcpServerClient::onSocketConnected()
{
	log_debug("Socket connected");
}

void BaseTcpServerClient::onSocketDisconnected()
{
	log_debug("Socket disconnected");
}

void BaseTcpServerClient::onSocketReadyRead()
{
	QByteArray data = m_tcpSocket->readAll();

	foreach (ITcpReceiver* receiver, m_receiversList) {
		receiver->onDataReceived(QVariant(data));
	}
}

void BaseTcpServerClient::onSocketDisplayError(QAbstractSocket::SocketError socketError)
{
	switch(socketError)
	{
		case QAbstractSocket::RemoteHostClosedError:
			//m_reconnectTimer->start(1000);
			log_error("Error! Connection with TCP device was lost");
			break;
		case QAbstractSocket::HostNotFoundError:
			log_error("Error! Host not found");
			break;
		case QAbstractSocket::ConnectionRefusedError:
			log_error("Error! Connection was refused");
			break;
		default:
			log_error(QString("Error! Error text: %1").arg(m_tcpSocket->errorString()));
			break;
	}
}

void BaseTcpServerClient::disconnectFromHostInternalSlot()
{
	m_reconnectTimer->stop();
    m_reconnectTimer->deleteLater();
	m_tcpSocket->disconnectFromHost();
	m_tcpSocket->abort();
}

void BaseTcpServerClient::writeDataInternalSlot(const QByteArray& data)
{
	m_tcpSocket->write(data);
}
