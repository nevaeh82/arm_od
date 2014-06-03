#include "TcpArmrClientController.h"

TcpArmrClientController::TcpArmrClientController(const QString &tcpDeviceName, QObject *parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	init();
	connect(this, SIGNAL(createTcpArmrCoderInternalSignal()), this, SLOT(createTcpArmrCoderInternalSlot()));
}

TcpArmrClientController::~TcpArmrClientController()
{
}

bool TcpArmrClientController::init()
{
	QSettings settings("./TCP/tcpArmrDevice.ini", QSettings::IniFormat, this);

	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();
		if(name == m_tcpDeviceName)
		{
			m_settings.host = settings.value("ip", "127.0.0.1").toString();
			m_settings.port = settings.value("port", 6662).toInt();
			m_settings.name = settings.value("name", "").toString();
			m_settings.reconnectInterval = settings.value("reconnectInterval", 1000).toInt();

			m_host = m_settings.host;
			m_port = m_settings.port;
			m_deviceType = TypeArmrTcpClient;

			QByteArray baseInfo;
			QDataStream dsBaseInfo(&baseInfo, QIODevice::WriteOnly);
			dsBaseInfo << m_settings;

			settings.endGroup();
			return true;
		}
		settings.endGroup();
	}
	return false;
}

void TcpArmrClientController::createTcpDeviceCoder()
{
	emit createTcpArmrCoderInternalSignal();
}

QObject *TcpArmrClientController::asQObject()
{
	return this;
}

void TcpArmrClientController::createTcpArmrCoderInternalSlot()
{
	log_debug("Creating TcpArmrCoder...");
	m_tcpDeviceCoder = new TcpArmrClientCoder(this);
}
