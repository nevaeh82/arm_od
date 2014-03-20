#include "TcpArmrClientCoder.h"
#include "../Common/Defines.h"

TcpArmrClientCoder::TcpArmrClientCoder(QObject *parent) :
	BaseTcpDeviceCoder(parent)
{
}

TcpArmrClientCoder::~TcpArmrClientCoder()
{
}

MessageSP TcpArmrClientCoder::encode(const QByteArray &data)
{
	Zaviruha::Packet packet;
	if (packet.ParseFromArray(data.data(), data.size())){
		QString method = "";
		switch (packet.command().action()){
			case Zaviruha::sendAtlantDirection:
				method = QString(ARM_R_SERVER_ATLANT_DIRECTION);
				break;
			case Zaviruha::sendAtlantPosition:
				method = QString(ARM_R_SERVER_ATLANT_POSITION);
				break;
			case Zaviruha::sendBplaPoints:
				method = QString(ARM_R_SERVER_BPLA_COORDS);
				break;
			case Zaviruha::sendBplaPointsAuto:
				method = QString(ARM_R_SERVER_BPLA_COORDS_AUTO);
				break;
			default:
				break;
		}

		return MessageSP(new Message<QByteArray>(method, data));
	}

	return MessageSP();
}

QByteArray TcpArmrClientCoder::decode(const MessageSP message)
{
	return message.data()->data();
}

QObject *TcpArmrClientCoder::asQObject()
{
	return this;
}
