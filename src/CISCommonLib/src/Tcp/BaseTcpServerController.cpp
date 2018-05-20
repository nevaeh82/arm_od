#include "BaseTcpServerController.h"

BaseTcpServerController::BaseTcpServerController(const QString& tcpServerName, QObject *parent) :
	QObject(parent)
{
	m_tcpServerName = tcpServerName;

	m_tcpServer = NULL;
	m_tcpServerCoder = NULL;
}

BaseTcpServerController::~BaseTcpServerController()
{
}

void BaseTcpServerController::onDataReceived(const QVariant &argument)
{
	if (NULL == m_tcpServerCoder) {
		log_debug(QString("m_tcpServerCoder is NULL"));
		return;
	}

	MessageSP message = m_tcpServerCoder->encode(argument.toByteArray());

	if (message.isNull()) {
//		log_debug(QString("message == NULL for %1").arg(m_tcpDeviceName));
		return;
	}

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(m_deviceType, m_tcpServerName, message);
	}
}

void BaseTcpServerController::createTcpServer()
{
	log_debug("Creating BaseTcpServer...");
	m_tcpServer = new BaseTcpServer(this);
	m_tcpServer->registerReceiver(this);
}

void BaseTcpServerController::createTcpServerCoder()
{
	log_debug("Creating BaseTcpServerCoder...");
	m_tcpServerCoder = new BaseTcpDeviceCoder(this);
}

void BaseTcpServerController::start(const QHostAddress &host, const quint16 port)
{
	if (NULL == m_tcpServer) {
		log_warning(QString("m_tcpServer is NULL"));
		return;
	}

	log_debug(QString("Starting BaseTcpServer %1:%2").arg(host.toString()).arg(port));
	m_tcpServer->start(host, port);
}

void BaseTcpServerController::stop()
{
	if (NULL == m_tcpServer) {
		log_warning(QString("m_tcpServer is NULL"));
		return;
	}

	m_tcpServer->stop();
}

bool BaseTcpServerController::isListening()
{
	if (NULL == m_tcpServer) {
		log_warning(QString("m_tcpServer is NULL"));
		return false;
	}

	return m_tcpServer->isListening();
}

bool BaseTcpServerController::sendData(const uint clientId, const MessageSP message)
{
	QByteArray decodedData = prepareDataToSend(message);

	if (decodedData.isEmpty()) {
		return false;
	}

	return m_tcpServer->sendData(clientId, decodedData);
}

bool BaseTcpServerController::sendData(const MessageSP message)
{
	QByteArray decodedData = prepareDataToSend(message);

	if (decodedData.isEmpty()) {
		return false;
	}

	return m_tcpServer->sendData(decodedData);
}

QObject *BaseTcpServerController::asQObject()
{
	return this;
}

QByteArray BaseTcpServerController::prepareDataToSend(const MessageSP message)
{
	if (NULL == m_tcpServerCoder) {
		log_warning(QString("m_tcpServerCoder is NULL"));
		return QByteArray();
	}

	if (NULL == m_tcpServer) {
		log_warning(QString("m_tcpServer is NULL"));
		return QByteArray();
	}

	QByteArray decodedData = m_tcpServerCoder->decode(message);

	if (decodedData.size() <= 0) {
		log_debug(QString("decodedData.size() <= 0 for %1").arg(m_tcpServerName));
	}

	return decodedData;
}
