#include "TcpAISController.h"

TcpAISController::TcpAISController(QObject *parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = DeviceTypes::AIS_TCP_DEVICE;
	log_debug(QString("Created %1").arg(m_tcpDeviceName));

	connect(this, SIGNAL(connectToHostAISInternalSignal(QString,quint32)), this, SLOT(connectToHostAISInternalSlot(QString,quint32)));
	connect(this, SIGNAL(createTcpAISCoderInternalSignal()), this, SLOT(createTcpAISCoderInternalSlot()));
	connect(this, SIGNAL(createTcpAISClientInternalSignal()), this, SLOT(createTcpAISClientInternalSlot()));

	m_zoneManager = new TcpAISZoneManager(this);
}

TcpAISController::TcpAISController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	init();
	connect(this, SIGNAL(connectToHostAISInternalSignal(QString,quint32)), this, SLOT(connectToHostAISInternalSlot(QString,quint32)));
	connect(this, SIGNAL(createTcpAISCoderInternalSignal()), this, SLOT(createTcpAISCoderInternalSlot()));
	connect(this, SIGNAL(createTcpAISClientInternalSignal()), this, SLOT(createTcpAISClientInternalSlot()));

	m_zoneManager = new TcpAISZoneManager(this);
}

TcpAISController::~TcpAISController()
{
}

void TcpAISController::createTcpClient()
{
	emit createTcpAISClientInternalSignal();
}

void TcpAISController::createTcpDeviceCoder()
{
	emit createTcpAISCoderInternalSignal();
}

void TcpAISController::connectToHost(const QString& host, const quint32& port)
{
	emit connectToHostAISInternalSignal(host, port);
}

void TcpAISController::disconnectFromHost()
{
}

QObject* TcpAISController::asQObject()
{
	return this;
}

void TcpAISController::connectToHostAISInternalSlot(const QString& host, const quint32& port)
{
	//60.7209, 28.0289, 58.8187, 34.0933

	QStringList stringList = host.split(":");
	if (stringList.size() != 4) {
		log_debug("Wrong square parametr");
		return;
	}

	double latitude1	= stringList.at(0).toDouble();
	double longitude1	= stringList.at(1).toDouble();
	double latitude2	= stringList.at(2).toDouble();
	double longitude2	= stringList.at(3).toDouble();

	QString __host = TCP_AIS_URL_STANDART;
	__host += m_zoneManager->getZone(latitude1, longitude1, latitude2, longitude2);

	m_tcpClient->connectToHost(__host, port);
}

void TcpAISController::createTcpAISCoderInternalSlot()
{
	log_debug("Creating TcpAISCoder...");
	m_tcpDeviceCoder = new TcpAISCoder(this);
}

void TcpAISController::createTcpAISClientInternalSlot()
{
	log_debug("Creating TcpAISClient...");
	m_tcpClient = new TcpAISClient(this);
	m_tcpClient->registerReceiver(this);
}

void TcpAISController::connectToHost()
{
	emit connectToHostAISInternalSignal(m_host, m_port);
}

bool TcpAISController::init()
{
	QSettings settings("./TCP/devices.ini", QSettings::IniFormat, this);

	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();
		if(name == m_tcpDeviceName)
		{
			m_AISSettingStruct.host = settings.value("ip", "127.0.0.1").toString();
			m_AISSettingStruct.port = settings.value("port", 2323).toInt();
			m_AISSettingStruct.name = settings.value("name", "").toString();

			m_host = m_AISSettingStruct.host;
			m_port = m_AISSettingStruct.port;
			m_deviceType = BaseSettingsType::TypeAIS;//m_flakonSettingStruct.type;

			QByteArray baseInfo;
			QDataStream dsBaseInfo(&baseInfo, QIODevice::WriteOnly);
			dsBaseInfo << m_AISSettingStruct;


			settings.endGroup();
			return true;
		}
		settings.endGroup();
	}
	return false;
}

QByteArray TcpAISController::getFullInfo()
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_AISSettingStruct;
	return ba;
}
