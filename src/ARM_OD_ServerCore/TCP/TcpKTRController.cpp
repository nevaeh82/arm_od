#include "TcpKTRController.h"

TcpKTRController::TcpKTRController(QObject* parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = DeviceTypes::KTR_TCP_DEVICE;
	m_deviceType = BaseSettingsType::TypeKTR;
	log_debug(QString("Created %1").arg(m_tcpDeviceName));
	connect(this, SIGNAL(createTcpKTRCoderInternalSignal()), this, SLOT(createTcpKTRCoderInternalSlot()));
}

TcpKTRController::TcpKTRController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	m_deviceType = BaseSettingsType::TypeKTR;
	init();
	connect(this, SIGNAL(createTcpKTRCoderInternalSignal()), this, SLOT(createTcpKTRCoderInternalSlot()));
	connect(m_tcpClient, SIGNAL(signalConnectedToHost(bool)), this, SLOT(tcpConnectionStatusInternalSlot(int)));
}

TcpKTRController::~TcpKTRController()
{
}

void TcpKTRController::createTcpDeviceCoder()
{
	emit createTcpKTRCoderInternalSignal();
}

void TcpKTRController::createTcpKTRCoderInternalSlot()
{
	log_debug("Creating TcpKTRCoder...");
	m_tcpDeviceCoder = new TcpKTRCoder(this);
}

void TcpKTRController::tcpConnectionStatusInternalSlot(int status)
{
	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << status;

	MessageSP message(new Message<QByteArray>(TCP_KTR_ANSWER_CONNECTION_STATUS, dataToSend));

	if (message == NULL) {
		return;
	}

	foreach (ITcpListener* listener, m_receiversList) {
		listener->onMessageReceived(m_deviceType, m_tcpDeviceName, message);
	}
}

bool TcpKTRController::init()
{
	QSettings settings("./TCP/devices.ini", QSettings::IniFormat, this);

	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();
		if(name == m_tcpDeviceName)
		{
			m_KTRSettingStruct.host = settings.value("ip", "127.0.0.1").toString();
			m_KTRSettingStruct.port = settings.value("port", 2323).toInt();
			m_KTRSettingStruct.name = settings.value("name", "").toString();

			m_host = m_KTRSettingStruct.host;
			m_port = m_KTRSettingStruct.port;
//			m_deviceType = BaseSettingsType::TypeKTR;//m_flakonSettingStruct.type;

			QByteArray baseInfo;
			QDataStream dsBaseInfo(&baseInfo, QIODevice::WriteOnly);
			dsBaseInfo << m_KTRSettingStruct;


			settings.endGroup();
			return true;
		}
		settings.endGroup();
	}
	return false;
}

QByteArray TcpKTRController::getFullInfo()
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_KTRSettingStruct;
	return ba;
}
