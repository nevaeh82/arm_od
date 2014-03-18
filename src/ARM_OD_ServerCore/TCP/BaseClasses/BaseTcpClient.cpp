#include "BaseTcpClient.h"

BaseTcpClient::BaseTcpClient(QObject* parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(BaseTcpClient));

	m_host = "127.0.0.1";
	m_port = 0;

	connect(this, SIGNAL(connectToHostInternalSignal(QString,quint32)), this, SLOT(connectToHostInternalSlot(QString,quint32)));
	connect(this, SIGNAL(disconnectFromHostInternalSignal()), this, SLOT(disconnectFromHostInternalSlot()));
	connect(this, SIGNAL(writeDataInternalSignal(QByteArray)), this, SLOT(writeDataInternalSlot(QByteArray)));

	m_reconnectTimer = new QTimer(this);
	connect(m_reconnectTimer, SIGNAL(timeout()), this, SLOT(reconnectSlot()));

	m_tcpSocket = new QTcpSocket(this);

	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
	connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketDisplayError(QAbstractSocket::SocketError)));
}

BaseTcpClient::~BaseTcpClient()
{
}

void BaseTcpClient::connectToHost(const QString& host, const quint32& port)
{
	emit connectToHostInternalSignal(host, port);
}

void BaseTcpClient::disconnectFromHost()
{
	emit disconnectFromHostInternalSignal();
}

bool BaseTcpClient::isConnected()
{
	return m_tcpSocket->state() == QAbstractSocket::ConnectedState;
}

void BaseTcpClient::writeData(const QByteArray& data)
{
	emit writeDataInternalSignal(data);
}

QString BaseTcpClient::getHost()
{
	QString host = m_host + ":" + QString::number(m_port);
	return host;
}

QObject* BaseTcpClient::asQObject()
{
	return this;
}

void BaseTcpClient::onSocketConnected()
{
	m_logger->debug("Socket connected");
}

void BaseTcpClient::onSocketDisconnected()
{
	m_logger->debug("Socket disconnected");
}

void BaseTcpClient::onSocketReadyRead()
{
//	m_logger->debug("Socket ready read");
	QByteArray data = m_tcpSocket->readAll();

	foreach (ITcpReceiver* receiver, m_receiversList) {
		receiver->onDataReceived(QVariant(data));
	}
}

void BaseTcpClient::onSocketDisplayError(QAbstractSocket::SocketError socketError)
{
	switch(socketError)
	{
		case QAbstractSocket::RemoteHostClosedError:
			m_reconnectTimer->start(1000);
			m_logger->debug("Error! Connection with TCP device was lost");
			break;
		case QAbstractSocket::HostNotFoundError:
			m_logger->debug("Error! Host not found");
			break;
		case QAbstractSocket::ConnectionRefusedError:
			m_logger->debug("Error! Connection was refused");
			break;
		default:
			m_logger->debug(QString("Error! Error text: %1").arg(m_tcpSocket->errorString()));
			break;
	}
}

void BaseTcpClient::connectToHostInternalSlot(const QString& host, const quint32& port)
{
	m_host = host;
	m_port = port;

	m_reconnectTimer->stop();
	m_tcpSocket->connectToHost(m_host, m_port);
	if (!m_tcpSocket->waitForConnected(300)) {
		m_logger->debug(QString("Connect failed to %1:%2").arg(host).arg(QString::number(port)));
	}
	m_logger->debug(QString("Connectection established to %1:%2").arg(host).arg(QString::number(port)));
}

void BaseTcpClient::disconnectFromHostInternalSlot()
{
	m_reconnectTimer->stop();
	m_tcpSocket->disconnectFromHost();
	m_tcpSocket->abort();
}

void BaseTcpClient::writeDataInternalSlot(const QByteArray& data)
{
	m_tcpSocket->write(data);
}
