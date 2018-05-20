#ifndef REPLYTIMEOUT_H
#define REPLYTIMEOUT_H

#include <QObject>
#include <QNetworkReply>

class ReplyTimeout : public QObject
{
	Q_OBJECT

public:
	ReplyTimeout(QNetworkReply* reply, const int timeout);

private slots:
	void timeout();
};

#endif // REPLYTIMEOUT_H
