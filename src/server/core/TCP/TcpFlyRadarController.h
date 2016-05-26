#pragma once

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

#include <QTimer>

#define FLYRADAR_REQUEST_TIMEOUT 1000

class TcpFlyRadarController : public QObject
{

	Q_OBJECT

public:

	TcpFlyRadarController(QObject* parent = 0);
	virtual ~TcpFlyRadarController();

private:
	QNetworkAccessManager* m_networkCore;
	QTimer* m_requestTimer;

private slots:
	void replyFinished(QNetworkReply*reply);

	void onRequest();

	void onStart();
	void onPause();
};


