#pragma once

#include <Tcp/BaseTcpDeviceController.h>

#include <QByteArray>

#include "ADSBPacket.pb.h"

class BaseADSBController : public BaseTcpDeviceController
{
	Q_OBJECT

public:
	explicit BaseADSBController(const QString& tcpDeviceName = BASE_TCP_DEVICE, QObject* parent = NULL);
	virtual ~BaseADSBController();

	virtual void enableAdsb(bool) = 0;

	QByteArray pack(const QList<ADSBData::Packet_Board>& list);
};

