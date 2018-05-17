#include "ReplyTimeout.h"

#include <QTimer>

ReplyTimeout::ReplyTimeout(QNetworkReply* reply, const int timeout)
	: QObject(reply)
{
	Q_ASSERT(reply);
	if (reply) {
		QTimer::singleShot(timeout, this, SLOT(timeout()));
	}
}

void ReplyTimeout::timeout()
{
	QNetworkReply* reply = static_cast<QNetworkReply*>(parent());
	if (reply->isRunning()) {
		reply->close();
	}
}
