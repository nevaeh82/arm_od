#include "BaseADSBController.h"

#include <QSettings>

BaseADSBController::BaseADSBController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
}

BaseADSBController::~BaseADSBController()
{
}

QByteArray BaseADSBController::pack(const QList<ADSBData::Packet_Board>& list)
{
	ADSBData::Packet packet;

	foreach(auto value, list) {
		packet.add_board()->CopyFrom(value);
	}

	QByteArray data;
	data.resize(packet.ByteSize());
	packet.SerializeToArray( data.data(), packet.ByteSize() );

	return data;
}
