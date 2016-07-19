#pragma once

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

#include <QTimer>
#include <QRectF>

#include <Tcp/BaseADSBController.h>

#define FLYRADAR_REQUEST_TIMEOUT 5000

class TcpFlyRadarController : public BaseADSBController
{

	Q_OBJECT

public:

	TcpFlyRadarController( const QString& tcpDeviceName = BASE_TCP_DEVICE, QObject* parent = NULL );
	virtual ~TcpFlyRadarController();

private:
	QNetworkAccessManager* m_networkCore;
	QTimer* m_requestTimer;

	double m_lonNR;
	double m_latNR;

	double m_lonSL;
	double m_latSL;

	quint64 m_ldv;

private slots:
	void replyFinished(QNetworkReply*reply);

	void onRequest();

	void onStart();
	void onPause();

public slots:
	void onInit();

	// BaseADSBController interface
public:
	virtual void enableAdsb(bool);

signals:
	void signalOnStart();
	void signalOnStop();
};


