#include "TcpAISController.h"

TcpAISController::TcpAISController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	init();
	connect(this, SIGNAL(connectToHostAISInternalSignal(QString,quint32)), this, SLOT(connectToHostAISInternalSlot(QString,quint32)));
	connect(this, SIGNAL(createTcpAISCoderInternalSignal()), this, SLOT(createTcpAISCoderInternalSlot()));
	connect(this, SIGNAL(createTcpAISClientInternalSignal()), this, SLOT(createTcpAISClientInternalSlot()));

	m_zoneManager = new TcpAISZoneManager(this);

	connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(updateDataInternalSlot()));
	m_updateTimer.start(TCP_AIS_TIMER_LIFE);
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

	AISZone zone;
	zone.latitudeMax	= stringList.at(0).toDouble();
	zone.longitudeMin	= stringList.at(1).toDouble();
	zone.latitudeMin	= stringList.at(2).toDouble();
	zone.longitudeMax	= stringList.at(3).toDouble();

	QString __host = TCP_AIS_URL_STANDART;
	__host += m_zoneManager->getZone(zone);

	m_tcpClient->connectToHost(__host, port);
}

void TcpAISController::createTcpAISCoderInternalSlot()
{
	log_debug("Creating TcpAISCoder...");
	m_tcpDeviceCoder = new TcpAISCoder(m_zoneManager, this);
}

void TcpAISController::createTcpAISClientInternalSlot()
{
	log_debug("Creating TcpAISClient...");
	m_tcpClient = new TcpAISClient(this);
	m_tcpClient->registerReceiver(this);
}

void TcpAISController::updateDataInternalSlot()
{
	connectToHost();
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
			m_deviceType = TypeAIS;//m_flakonSettingStruct.type;

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

void TcpAISController::sendData(const MessageSP message)
{
	QString messageType = message->type();
	if (messageType == TCP_AIS_REQUEST_GET_DATA) {
		connectToHost();
	}
}
