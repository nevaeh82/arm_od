#ifndef QPROGRESSNETWORKACCESSMANAGER_H
#define QPROGRESSNETWORKACCESSMANAGER_H

#include <QtNetwork/QNetworkAccessManager>

class QProgressNetworkAccessManager : public QNetworkAccessManager
{
	Q_OBJECT
public:
	QProgressNetworkAccessManager(QObject *parent = 0);

signals:
	void loadProgress ( int progress );

protected:
	virtual QNetworkReply * createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);

private:
	qint32 requestsCounter;
	qint32 finishedCounter;

private slots:
	void finishedSlot( QNetworkReply * reply );
};

#endif // QPROGRESSNETWORKACCESSMANAGER_H
