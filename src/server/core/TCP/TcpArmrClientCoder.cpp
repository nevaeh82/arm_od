#include "TcpArmrClientCoder.h"
#include "../Common/Defines.h"
#include "TCP/TcpDefines.h"

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
	if (packet.ParseFromArray(data.data() + TCP_ZAVIRUHA_PREAMBULA_LEN, data.size() - TCP_ZAVIRUHA_PREAMBULA_LEN)){
		QString method = "";
		QByteArray dataToSend = data;
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
			/// todo: Update CISCommonLib before
//			case Zaviruha::sendBplaPointsSingle:
//				method = QString(ARM_R_SERVER_BPLA_COORDS_SINGLE);
//				break;
			default:
				break;
		}

		return MessageSP(new Message<QByteArray>(method, dataToSend));
	}

	return MessageSP();
}

QByteArray TcpArmrClientCoder::decode(const MessageSP message)
{
	QString messageType = message->type();

	Zaviruha::Packet packet;
	Zaviruha::Packet::Command* packetCommand = new Zaviruha::Packet::Command();
	packet.set_allocated_command(packetCommand);

	if (messageType == TCP_ARMR_SEND_SOLVER_DATA) {
		packetCommand->set_action(Zaviruha::sendSolverData);

		Zaviruha::Packet::ArgumentVariant* packetArgs = new Zaviruha::Packet::ArgumentVariant();
		Zaviruha::Packet::ArgumentVariant::SolverData* solverData = packetArgs->add_solverdata();
		packetCommand->set_allocated_arguments(packetArgs);

		QByteArray inputData = message->data();

		int id = 0;
		int trackLength = 0;
		double altitude = 0;

		QDataStream dataStream(&inputData, QIODevice::ReadOnly);
		dataStream >> id >> trackLength >> altitude;

		solverData->set_id(id);
		solverData->set_tracklength(trackLength);
		solverData->set_altitude(altitude);
	}
	else if (messageType == TCP_ARMR_SEND_SOLVER_CLEAR) {
		packetCommand->set_action(Zaviruha::sendSolverClear);
	} else if (messageType == TCP_ARMR_SEND_KTR_DATA) {
		packetCommand->set_action(Zaviruha::sendKtrAction);
	}

	QByteArray dataToSend;
	unsigned int size = packet.ByteSize();
	dataToSend.resize(size);
	packet.SerializeToArray(dataToSend.data(), size);

	dataToSend.prepend((char*)&size, sizeof(unsigned int));
	dataToSend.prepend(TCP_ZAVIRUHA_PREAMBULA);

	return dataToSend;
}

QObject *TcpArmrClientCoder::asQObject()
{
	return this;
}
