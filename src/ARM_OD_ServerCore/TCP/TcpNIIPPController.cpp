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

void TcpNIIPPController::createTcpNIIPPCoderInternalSlot()
{
	log_debug("Creating TcpNIIPPCoder...");
	m_tcpDeviceCoder = new TcpNIIPPCoder(m_id, this);

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
			m_NIIPPSettingStruct.reconnectInterval = settings.value("reconnectInterval", 1000).toInt();

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
