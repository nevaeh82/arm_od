#include "BaseTcpClient.h"

#include "Sleeper.h"

#define BASE_TCP_RECONNECT_TIMEOUT 3000

BaseTcpClient::BaseTcpClient(QObject* parent, const bool isReadRequired):
	QIODevice( parent ),
	m_reconnectTimer(0),
	m_reconnectTimeout( BASE_TCP_RECONNECT_TIMEOUT )
{
	m_host = "127.0.0.1";
	m_port = 0;

	connect(this, SIGNAL(writeDataInternalSignal(QByteArray)), this, SLOT(writeDataInternalSlot(QByteArray)));

    m_tcpSocket = new QTcpSocket(this);

	if (isReadRequired) {
		log_debug("isReadRequired == true");
		connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
	}
	connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketDisplayError(QAbstractSocket::SocketError)));

	connect(m_tcpSocket, SIGNAL(readyRead()), this, SIGNAL(readyRead()));

    connect(this, SIGNAL(signalKillTimer()), this, SLOT(killTimerSlot()));

}

BaseTcpClient::~BaseTcpClient()
{
	log_debug("~BaseTcpClient");
}

bool BaseTcpClient::open(QIODevice::OpenMode mode)
{
	return QIODevice::open(mode);
}

void BaseTcpClient::close()
{
	m_tcpSocket->abort();

	QIODevice::close();
}

qint64 BaseTcpClient::bytesAvailable() const
{
	return m_tcpSocket->bytesAvailable();
}

qint64 BaseTcpClient::readData(char* data, qint64 maxlen)
{
	return m_tcpSocket->read(data, maxlen);
}

qint64 BaseTcpClient::writeData(const char* data, qint64 len)
{
	emit writeDataInternalSignal(QByteArray(data, len));

	return len;
}


void BaseTcpClient::setReconnectInterval(const uint timeMsec)
{
	m_reconnectTimeout = timeMsec;
}


void BaseTcpClient::connectToHostAsync(const QString& host, const quint32 port)
{
	qRegisterMetaType<quint32>("quint32");
	QMetaObject::invokeMethod(this, "connectToHostInternalSlot", Qt::QueuedConnection,
							  Q_ARG(QString, host),
							  Q_ARG(quint32, port));
}

void BaseTcpClient::disconnectFromHostAsync()
{
	QMetaObject::invokeMethod( this, "disconnectFromHostInternalSlot", Qt::QueuedConnection );
	Sleeper::msleep(200);
}

bool BaseTcpClient::connectToHostSync(const QString &host, const quint32 port)
{
	connectToHostInternalSlot(host, port);

	return isConnected();
}

bool BaseTcpClient::disconnectFromHostSync()
{
	disconnectFromHostInternalSlot();
	return isConnected();
}

bool BaseTcpClient::isConnected()
{
//	log_debug("Connection state = " + QString::number(m_tcpSocket->state()));
	return (isOpen() && m_tcpSocket->state() == QAbstractSocket::ConnectedState);
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

void BaseTcpClient::killTimerSlot()
{
    if (m_reconnectTimer != 0) {
        killTimer(m_reconnectTimer);
        m_reconnectTimer = 0;
    }
}

void BaseTcpClient::onSocketConnected()
{
	log_debug("Socket connected");

    emit signalKillTimer();

	open(QIODevice::ReadWrite);
	emit signalConnectedToHost(TCP::Connected);
}

void BaseTcpClient::onSocketDisconnected()
{
	log_debug("Socket disconnected");
	QIODevice::close();
	emit signalConnectedToHost(TCP::Disconnected);
}

void BaseTcpClient::onSocketReadyRead()
{
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

	QIODevice::close();

	log_info(QString("Host = %1, Port = %2").arg(m_host).arg(m_port));
	if (m_reconnectTimer == 0) {
		m_reconnectTimer = startTimer(m_reconnectTimeout);
	}
}

void BaseTcpClient::connectToHostInternalSlot(QString host, quint32 port)
{
	log_debug("Connecting...");
	m_host = host;
	m_port = port;

    emit signalKillTimer();

	m_tcpSocket->connectToHost(m_host, m_port);
	if (!m_tcpSocket->waitForConnected(1000)) {
		log_error(QString("Connect failed to %1:%2").arg(host).arg(QString::number(port)));
		emit signalConnectedToHost(TCP::Disconnected);
		m_reconnectTimer = startTimer(m_reconnectTimeout);
		return;
	}

	log_info(QString("Connectection established to %1:%2").arg(host).arg(QString::number(port)));
}

void BaseTcpClient::disconnectFromHostInternalSlot()
{
	log_debug("Disconnecting...");

    emit signalKillTimer();

	close();
}

void BaseTcpClient::writeDataInternalSlot(const QByteArray& data)
{
	m_tcpSocket->write(data);
}

void BaseTcpClient::onSocketReconnectSlot()
{
	log_info("Reconnecting...");
	if(m_tcpSocket->state() != QAbstractSocket::UnconnectedState) {
		log_error("Get valid socket state, exit");
		return;
	}

	close();

	m_tcpSocket->connectToHost(m_host, m_port);
	m_tcpSocket->waitForConnected(1000);
}

#include <QTimerEvent>

void BaseTcpClient::timerEvent(QTimerEvent* e)
{
	if (e->timerId() == m_reconnectTimer) {
		onSocketReconnectSlot();
	}
}
