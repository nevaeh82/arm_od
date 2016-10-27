#pragma once

#include <QObject>
#include <QUdpSocket>

#include "Tcp/BaseTcpClient.h"

class KTPSendNMEA : public BaseTcpClient
{
	Q_OBJECT
private:
	QUdpSocket* m_udpSocket;
	quint16 m_packetNumber;
	QByteArray buffer;
	static const QByteArray m_preambula;
	quint16 m_board;

	double latitude;
	double longitude;
	quint32 altitude;
	quint32 course;

	void sendToKtp(quint32, const QByteArray&);

	QString m_ktrIp;

	double convertNMEAtoDecimal(const double&);
public:
	KTPSendNMEA(QObject *parent = NULL);
	~KTPSendNMEA();

	QString getKtrIp();

public slots:
	void onConnect(const QString& ktrIp);
	void onDisconnect();

	void sendDataToKtp(QString ktrIp, uint id, uint port, const QByteArray& inputData);

private slots:
	void onConnectedSlot();
	void onSocketReadyReadSlot();

signals:
	void onBoardDataReceived(QString ip, quint32 id, quint32 port, QByteArray data);
    void onGpsDataReceived(QByteArray data);
};
