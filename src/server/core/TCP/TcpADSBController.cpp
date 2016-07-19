#include "TcpADSBController.h"

#include <QSettings>

TcpADSBController::TcpADSBController(const QString& tcpDeviceName, QObject* parent) :
	BaseADSBController(tcpDeviceName, parent),
	m_client(NULL)
{
	connect(this, SIGNAL(signalStart()), this, SLOT(onStart()), Qt::QueuedConnection);
	connect(this, SIGNAL(signalStop()), this, SLOT(onStop()), Qt::QueuedConnection);

	QSettings settings("ADSB.ini", QSettings::IniFormat);
	m_processPath = settings.value("ADSB/processPath", "dump1090").toString();
	m_processName = settings.value("ADSB/processName", "dump1090.exe").toString();

	m_adsbHost = settings.value("ADSB/adsbHost", "192.168.24.29").toString();
	m_adsbPort = settings.value("ADSB/adsbPort", 30005).toUInt();
	m_riPort = settings.value("ADSB/riPort", 30001).toUInt();
	m_sbsPort = settings.value("ADSB/sbsPort", 30003).toUInt();
}

TcpADSBController::~TcpADSBController()
{
	onStop();
}

void TcpADSBController::enableAdsb(bool stat)
{
	if(stat) {
		emit signalStart();
	} else {
		emit signalStop();
	}
}

void TcpADSBController::onStart()
{
	if(!m_client) {
		return;
	}

	m_client->on_connectToDongleCheckBox_toggled(	true,
													m_processPath,
													m_processName,
													m_adsbHost,
													m_adsbPort,
													m_riPort,
													m_sbsPort );
}

void TcpADSBController::onStop()
{
	if(!m_client) {
		return;
	}

	m_client->on_connectToDongleCheckBox_toggled( false, m_processPath, m_processName );
}

void TcpADSBController::onInitPlanesClient()
{
	m_client = new PlanesClient();
	connect(m_client, SIGNAL(signal_PlaneDataChanged(QByteArray)), this, SLOT(onPlanesDataReceived(QByteArray)));
	//onStart();
}

void TcpADSBController::onPlanesDataReceived(QByteArray info)
{
	QDataStream ds(&info, QIODevice::ReadOnly);

	QList<ADSBData::Packet_Board> packetList;
	ADSBData::Packet_Board packet;

	QString id;
	double lon;
	double lat;
	quint32 speed;
	quint32 alt;

	ds >> id;
	ds >> lon;
	ds >> lat;
	ds >> alt;
	ds >> speed;

	packet.set_id(id.toAscii().data(), id.size());
	packet.set_lon(lon);
	packet.set_lat(lat);
	packet.set_alt(alt);
	packet.set_speed(speed);

	packetList.append(packet);

	QByteArray outArray = pack(packetList);;

	foreach (auto receiver, m_receiversList) {
		receiver->onMessageReceived(DeviceTypes::ADSB_TCP_CLIENT, m_tcpDeviceName,
									MessageSP(new Message<QByteArray>(TCP_ADSB_ANSWER_DATA, outArray)) );
	}
}
