#include "BaseTcpDeviceController.h"


BaseTcpDeviceController::BaseTcpDeviceController(const QString& tcpDeviceName, QObject* parent) :
	QObject(parent)
{
	m_tcpClient = NULL;
	m_tcpDeviceCoder = NULL;
	m_tcpDeviceName = tcpDeviceName;

	m_port = 0;
	m_deviceType = 0;

	log_debug(QString("Created %1").arg(m_tcpDeviceName));
}

BaseTcpDeviceController::~BaseTcpDeviceController()
{
	if (NULL == m_tcpClient) {
		log_warning(QString("m_tcpClient is NULL"));
		return;
	}

	m_tcpClient->deregisterReceiver(this);
}

void BaseTcpDeviceController::createTcpClient()
{
	log_debug("Creating BaseTcpClient...");
	m_tcpClient = new BaseTcpClient(this);
	m_tcpClient->registerReceiver(this);
	connect(m_tcpClient, SIGNAL(signalConnectedToHost(int)), this, SIGNAL(signalTcpDeviceConnectedToHost(int)));
}

void BaseTcpDeviceController::createTcpDeviceCoder()
{
	log_debug("Creating BaseTcpDeviceCoder...");
	m_tcpDeviceCoder = new BaseTcpDeviceCoder(this);
}

void BaseTcpDeviceController::connectToHost(const QString& host, const quint32& port)
{
	if (NULL == m_tcpClient) {
		log_warning(QString("m_tcpClient is NULL"));
		return;
	}

	m_tcpClient->connectToHostAsync(host, port);
}

void BaseTcpDeviceController::connectToHost()
{
	connectToHost(m_host, m_port);
}

void BaseTcpDeviceController::disconnectFromHost()
{
	m_tcpClient->disconnectFromHostAsync();
}

bool BaseTcpDeviceController::isConnected()
{
	if (NULL == m_tcpClient) {
		log_warning(QString("m_tcpClient is NULL"));
		return false;
	}

	return m_tcpClient->isConnected();
}

QString BaseTcpDeviceController::getHost()
{
	if (NULL == m_tcpClient) {
		log_warning(QString("m_tcpClient is NULL"));
		return QString();
	}

	return m_tcpClient->getHost();
}

void BaseTcpDeviceController::sendData(const MessageSP message)
{
	if (NULL == m_tcpDeviceCoder) {
		log_warning(QString("m_tcpDeviceCoder is NULL"));
		return;
	}

	QByteArray decodedData = m_tcpDeviceCoder->decode(message);

	if (decodedData.size() <= 0) {
		log_debug(QString("decodedData.size() <= 0 for %1").arg(m_tcpDeviceName));
		return;
	}

	if (NULL == m_tcpClient) {
		log_warning(QString("m_tcpClient is NULL"));
		return;
	}

	m_tcpClient->write(decodedData);
}

QObject* BaseTcpDeviceController::asQObject()
{
	return this;
}

bool BaseTcpDeviceController::init()
{
	return false;
}

void BaseTcpDeviceController::setDeviceInfo(const QByteArray& baseInfo, const QByteArray& fullInfo)
{
	Q_UNUSED(baseInfo);
	Q_UNUSED(fullInfo);
	/// Realized in inherited classes
}

QByteArray BaseTcpDeviceController::getFullInfo()
{
	/// Realized in inherited classes
	return QByteArray();
}

void BaseTcpDeviceController::onDataReceived(const QVariant& argument)
{
	if (NULL == m_tcpDeviceCoder) {
		log_warning(QString("m_tcpDeviceCoder is NULL"));
		return;
	}

    MessageSP message = m_tcpDeviceCoder->encode(argument.toByteArray());

    if(!message || message.isNull() || message->data().isEmpty()) {
        return;
    }

    if (!message.isNull() && NULL != m_tcpClient) {
        foreach (ITcpListener* receiver, m_receiversList) {
            receiver->onMessageReceived(m_deviceType, m_tcpDeviceName, message);
        }
    }

    while (true) {

        MessageSP message = m_tcpDeviceCoder->encode(QByteArray());

        if(!message || message.isNull() || message->data().isEmpty()) {
            return;
        }

        if (NULL == m_tcpClient) {
            log_warning(QString("m_tcpClient is NULL"));
            return;
        }

        foreach (ITcpListener* receiver, m_receiversList) {
            receiver->onMessageReceived(m_deviceType, m_tcpDeviceName, message);
        }
    }
}

void BaseTcpDeviceController::setReconnectInterval(const uint timeMsec)
{
	if (NULL != m_tcpClient) {
		m_tcpClient->setReconnectInterval(timeMsec);
	}
}

