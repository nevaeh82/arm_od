#include "TcpFlyRadarController.h"

#include "qt-json/json.h"

#include "Logger/Logger.h"

TcpFlyRadarController::TcpFlyRadarController(QObject *parent)
{
	m_networkCore = new QNetworkAccessManager(this);
	connect(m_networkCore, SIGNAL(finished(QNetworkReply*)),
			this, SLOT(replyFinished(QNetworkReply*)));

	m_requestTimer = new QTimer(this);
	m_requestTimer->setInterval(FLYRADAR_REQUEST_TIMEOUT);
	connect(m_requestTimer, SIGNAL(timeout()), this, SLOT(onRequest()));

	m_requestTimer->start();
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

	QMap<QString, QVariant> listRes2_1;
	QMap<QString, QVariant> listRes2_2;
	QMap<QString, QVariant> listRes2_3;

	QMap<QString, QVariant> listRes3;

	int i = 0;
	foreach (QString key, listRes1.keys()) {
		if(i == 0) {
			listRes2_1 = listRes1.value(key).toMap();
		} else if(i == 1) {
			listRes2_2 = listRes1.value(key).toMap();
		} else if(i == 2) {
			listRes2_3 = listRes1.value(key).toMap();
		}

		log_debug(QString("Decoded plane!!! %1").arg(key));

		i++;
	}
//	QList<QVariant> listRes2_2 = listRes1.at(1).toList();
//	QList<QVariant> listRes2_3 = listRes1.at(2).toList();

	listRes3 = listRes2_3.value("0").toMap();

	foreach (QString key, listRes3.keys()) {
		log_debug(QString("plane keys!!! %1").arg(key));
		QVariantList planesDataList = listRes3.value(key).toList();
	}

	int k = 0;
	k++;
}

void TcpFlyRadarController::onRequest()
{
	m_networkCore->get(QNetworkRequest(QUrl("https://planefinder.net/endpoints/update.php?faa=1&routetype=iata&cfCache=true&bounds=40.1,28,72.1,144&_=0")));
}

void TcpFlyRadarController::onStart()
{
	m_requestTimer->start();
}

void TcpFlyRadarController::onPause()
{
	m_requestTimer->stop();
}
