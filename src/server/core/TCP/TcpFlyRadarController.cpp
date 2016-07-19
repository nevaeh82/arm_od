#include "TcpFlyRadarController.h"

#include <QSettings>

#include "Tcp/TcpDefines.h"
#include "Tcp/TcpDevicesDefines.h"

#include "qt-json/json.h"
#include "Logger/Logger.h"

TcpFlyRadarController::TcpFlyRadarController(const QString& tcpDeviceName, QObject* parent ) :
	BaseADSBController(tcpDeviceName, parent)
{
	QSettings settings("ADSB.ini", QSettings::IniFormat);
	m_lonNR = settings.value("BBOX/lonNR", 0.0).toDouble();
	m_latNR = settings.value("BBOX/latNR", 0.0).toDouble();

	m_lonSL = settings.value("BBOX/lonSL", 0.0).toDouble();
	m_latSL = settings.value("BBOX/latSL", 0.0).toDouble();

	connect(this, SIGNAL(signalOnStart()), this, SLOT(onStart()), Qt::QueuedConnection);
	connect(this, SIGNAL(signalOnStop()), this, SLOT(onPause()), Qt::QueuedConnection);

	m_ldv = 0;
}

TcpFlyRadarController::~TcpFlyRadarController()
{

}

void TcpFlyRadarController::replyFinished(QNetworkReply* reply)
{
	QByteArray data = reply->readAll();

	bool success = false;
	QVariant res = QtJson::parse(QString(data), success);

	QMap<QString, QVariant> listRes1 = res.toMap();
	QVariantList dataList;

	QMap<QString, QVariant> listRes2;
	QList<ADSBData::Packet_Board> planeList;

	dataList = listRes1.value("acList").toList();
	m_ldv = listRes1.value("lastDv").toULongLong();

	foreach (QVariant data, dataList) {
		listRes2 = data.toMap();

		ADSBData::Packet_Board packet;
		packet.set_id( listRes2.value("Call").toByteArray().data() );
		packet.set_lat( listRes2.value("Lat").toDouble() );
		packet.set_lon( listRes2.value("Long").toDouble() );
		packet.set_alt( listRes2.value("GAlt").toDouble() );
		packet.set_yaw( listRes2.value("Trak").toDouble() );

		planeList.append( packet );
	}

	QByteArray info = pack( planeList );

	foreach (auto receiver, m_receiversList) {
		receiver->onMessageReceived(DeviceTypes::ADSB_TCP_CLIENT, m_tcpDeviceName,
									MessageSP(new Message<QByteArray>(TCP_ADSB_ANSWER_DATA, info)) );
	}

//	foreach (auto receiver, m_receiversList) {
//		receiver->onMessageReceived(DeviceTypes::ADSB_TCP_CLIENT, m_tcpDeviceName,
//									MessageSP(new Message<QByteArray>(TCP_ADSB_ANSWER_DATA, info)) );
//	}
}

void TcpFlyRadarController::onRequest()
{
//	QString request = QString( "https://api.rb24.com/flights?params={\"bounds\":{\"northEast\":{\"lat\":%1,\"lon\":%2},\"southWest\":{\"lat\":%3,\"lon\":%4}}}&fch=false&flightid=false&airline=false&station=false&aircraft=false&airport=false&\"airportorg=false&airportdst=false&fn=false&far=false&fms=false&adsb=true&asdi=true&ocea=true" ).
//					arg(m_latNR).arg(m_lonNR).
//					arg(m_latSL).arg(m_lonSL);

	QString request;

	if(m_ldv == 0) {
		request = QString("http://global.adsbexchange.com/VirtualRadar/AircraftList.json?fNBnd=%1&fEBnd=%2&fSBnd=%3&fWBnd=%4").
						arg(m_latNR).arg(m_lonNR).
						arg(m_latSL).arg(m_lonSL);
	} else {
		request = QString("http://global.adsbexchange.com/VirtualRadar/AircraftList.json?ldv=%5&fNBnd=%1&fEBnd=%2&fSBnd=%3&fWBnd=%4").
						arg(m_latNR).arg(m_lonNR).
						arg(m_latSL).arg(m_lonSL).
						arg(m_ldv);
	}


	m_networkCore->get(QNetworkRequest(QUrl(request)));
}

void TcpFlyRadarController::onStart()
{
	m_requestTimer->start();
}

void TcpFlyRadarController::onPause()
{
	m_requestTimer->stop();
}

void TcpFlyRadarController::onInit()
{
	m_networkCore = new QNetworkAccessManager(this);
	connect(m_networkCore, SIGNAL(finished(QNetworkReply*)),
			this, SLOT(replyFinished(QNetworkReply*)));

	m_requestTimer = new QTimer(this);
	m_requestTimer->setInterval(FLYRADAR_REQUEST_TIMEOUT);
	connect(m_requestTimer, SIGNAL(timeout()), this, SLOT(onRequest()));

	//onStart();
}


void TcpFlyRadarController::enableAdsb(bool val)
{
	if(val) {
		emit signalOnStart();
	} else {
		emit signalOnStop();
	}
}
