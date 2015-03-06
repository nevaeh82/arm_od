#include "TcpNIIPPController.h"

TcpNIIPPController::TcpNIIPPController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	init();
	connect(this, SIGNAL(createTcpNIIPPCoderInternalSignal()), this, SLOT(createTcpNIIPPCoderInternalSlot()));
}

TcpNIIPPController::~TcpNIIPPController()
{
}

void TcpNIIPPController::createTcpDeviceCoder()
{
	emit createTcpNIIPPCoderInternalSignal();
}

void TcpNIIPPController::createTcpClient()
{
	BaseTcpDeviceController::createTcpClient();
	m_tcpClient->setReconnectInterval(m_NIIPPSettingStruct.reconnectInterval);
	connect(m_tcpClient, SIGNAL(signalConnectedToHost(int)), this, SLOT(connectedToHostInternalSlot(int)));
}

void TcpNIIPPController::createTcpNIIPPCoderInternalSlot()
{
	log_debug("Creating TcpNIIPPCoder...");
	m_tcpDeviceCoder = new TcpNIIPPCoder(m_id, this);

}

void TcpNIIPPController::connectedToHostInternalSlot(int status)
{
	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	dataStream << m_id;
	dataStream << status;

	MessageSP message(new Message<QByteArray>(TCP_NIIPP_ANSWER_CONNECTION_STATUS, dataToSend));

	if (message == NULL) {
		return;
	}

	foreach (ITcpListener* listener, m_receiversList) {
		listener->onMessageReceived(m_deviceType, m_tcpDeviceName, message);
	}
}

bool TcpNIIPPController::init()
{
	QSettings settings("./TCP/devices.ini", QSettings::IniFormat, this);

	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();
		if(name == m_tcpDeviceName)
		{
			m_NIIPPSettingStruct.host = settings.value("ip", "127.0.0.1").toString();
			m_NIIPPSettingStruct.port = settings.value("port", 2323).toInt();
			m_NIIPPSettingStruct.name = settings.value("name", "").toString();
			m_NIIPPSettingStruct.reconnectInterval = settings.value("reconnectInterval", 3000).toInt();

			m_id = settings.value("id", 0).toInt();

			m_host = m_NIIPPSettingStruct.host;
			m_port = m_NIIPPSettingStruct.port;
			m_deviceType = TypeNIIPP;//m_flakonSettingStruct.type;

			QByteArray baseInfo;
			QDataStream dsBaseInfo(&baseInfo, QIODevice::WriteOnly);
			dsBaseInfo << m_NIIPPSettingStruct;

			settings.endGroup();
			return true;
		}
		settings.endGroup();
	}
	return false;
}

QByteArray TcpNIIPPController::getFullInfo()
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_NIIPPSettingStruct;
	return ba;
}
