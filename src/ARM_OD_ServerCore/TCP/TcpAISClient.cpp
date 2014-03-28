#include "TcpAISClient.h"

TcpAISClient::TcpAISClient(QObject* parent) :
	BaseTcpClient(parent)
{
	connect(this, SIGNAL(connectToHostAISInternalSignal(QString,quint32)), this, SLOT(connectToHostAISInternalSlot(QString,quint32)));

	/// Because they are instansed in BaseTcpClient
	if (m_reconnectTimer != NULL) {
		delete m_reconnectTimer;
		m_tcpSocket = NULL;
	}

	if (m_tcpSocket != NULL) {
		delete m_tcpSocket;
		m_tcpSocket = NULL;
	}

	m_networkAccessManager = new QNetworkAccessManager();
	connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGetQuerySlot(QNetworkReply*)));
}

TcpAISClient::~TcpAISClient()
{
}


void TcpAISClient::connectToHost(const QString& host, const quint32& port)
{
	emit connectToHostAISInternalSignal(host, port);
}

void TcpAISClient::disconnectFromHost()
{
}

bool TcpAISClient::isConnected()
{
	return true;
}

void TcpAISClient::writeData(const QByteArray& data)
{
}

QString TcpAISClient::getHost()
{
	return m_host + ":" + m_port;
}

QObject* TcpAISClient::asQObject()
{
	return this;
}

void TcpAISClient::onGetQuerySlot(QNetworkReply* reply)
{
	QString answer = QString::fromUtf8(reply->readAll());

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << answer;

	foreach (ITcpReceiver* receiver, m_receiversList) {
		receiver->onDataReceived(QVariant(dataToSend));
	}
}

void TcpAISClient::connectToHostAISInternalSlot(const QString& host, const quint32& port)
{
	m_host = host;
	m_port = port; // NULL

	m_networkAccessManager->get(QNetworkRequest(QUrl(m_host)));
}
