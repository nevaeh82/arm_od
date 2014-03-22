#include "TcpAISController.h"

TcpAISController::TcpAISController(QObject *parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = AIS_TCP_DEVICE;
	debug(QString("Created %1").arg(m_tcpDeviceName));
	connect(this, SIGNAL(createTcpAISCoderInternalSignal()), this, SLOT(createTcpAISCoderInternalSlot()));
	connect(this, SIGNAL(createTcpAISClientInternalSignal()), this, SLOT(createTcpAISClientInternalSlot()));

	m_zoneManager = new TcpAISZoneManager(this);
}

TcpAISController::TcpAISController(const QString& tcpDeviceName, QObject* parent)
{
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
	//60.7209, 28.0289, 58.8187, 34.0933

	QStringList stringList = host.split(":");
	if (stringList.size() != 4) {
		debug("Wrong square parametr");
		return;
	}

	double latitude1	= stringList.at(0).toDouble();
	double longitude1	= stringList.at(1).toDouble();
	double latitude2	= stringList.at(2).toDouble();
	double longitude2	= stringList.at(3).toDouble();

	QString __host = m_zoneManager->getZone(latitude1, longitude1, latitude2, longitude2);

	m_tcpClient->connectToHost(__host, port);
}

void TcpAISController::disconnectFromHost()
{
}

QObject* TcpAISController::asQObject()
{
	return this;
}

void TcpAISController::createTcpAISCoderInternalSlot()
{
	debug("Creating TcpAISCoder...");
	m_tcpDeviceCoder = new TcpAISCoder(this);
}

void TcpAISController::createTcpAISClientInternalSlot()
{
	debug("Creating TcpAISClient...");
	m_tcpClient = new TcpAISClient(this);
}
