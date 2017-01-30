#include "KTPSendNMEA.h"

#include "CTRL.pb.h"
#include "ByteArrayData.pb.h"
#include "EXNGLink.pb.h"
#include "RfClientsList.pb.h"
#include "GPS.pb.h"

#include "Logger/Logger.h"

#include <QtGlobal>
#include <QtEndian>

const QByteArray KTPSendNMEA::m_preambula = QByteArray::fromHex("b13fce61");

#define KTR_EXNG_PORT 39815

KTPSendNMEA::KTPSendNMEA(QObject *parent) :
	BaseTcpClient(parent, false),
	m_ktrIp("")
{
	m_packetNumber = 0;

	longitude = 0.0;
	latitude = 0.0;
	altitude = 0;
	course = 0;
}

KTPSendNMEA::~KTPSendNMEA()
{
}

QString KTPSendNMEA::getKtrIp()
{
	return m_ktrIp;
}

void KTPSendNMEA::onConnect(const QString& ktrIp)
{
	m_ktrIp = ktrIp;

	connectToHostAsync(ktrIp, KTR_EXNG_PORT);
	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyReadSlot()));
	connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(onConnectedSlot()));
}

void KTPSendNMEA::onDisconnect()
{
	BaseTcpClient::disconnectFromHostSync();
}

void KTPSendNMEA::onSocketReadyReadSlot()
{
    quint32 dt = m_tcpSocket->bytesAvailable();
    Q_UNUSED(dt)
	buffer.append(m_tcpSocket->readAll());

	while (static_cast<size_t>(buffer.size()) > 8) {
		int pos = 0;
		while (*(quint32*)(buffer.data() + pos) != *(quint32*)m_preambula.data() && pos < buffer.size() - 3) {
			++pos;
		}

		buffer.remove(0, pos);

		if (static_cast<size_t>(buffer.size()) < 8)
			return;

		buffer.remove(0,4);
		quint16 size_proto = qFromBigEndian(*(quint16*)buffer.mid(2,4).data());
		buffer.remove(0,4);

		CtrlLinePacket ctrllinepacket;
		bool result = ctrllinepacket.ParseFromArray(buffer.data(),size_proto );

		if (!result) {
			log_info("fail parse");
			continue;
		}

		if (!ctrllinepacket.IsInitialized()){
			log_info("not initialized");
			continue;
		}

		if (!ctrllinepacket.has_type()){
			log_info("type not set");
			continue;
		}

		if (!ctrllinepacket.has_data()){
			log_info("data not set");
			continue;
		}

        if( (quint32)ctrllinepacket.type() == CtrlLinePacket_PacketType_GPS_REPLY ) {
            KTP_3_Proto::GPSPacket data;
            QByteArray gpsData(ctrllinepacket.data().data(), ctrllinepacket.data().size());

            if (!data.ParseFromArray(ctrllinepacket.data().data(), ctrllinepacket.data().size())) {
                log_info("Failed parse KTRDataPacket-GPS");
            }

            emit onGpsDataReceived( gpsData );
        }

//		if( (quint32)ctrllinepacket.type() == CtrlLinePacket_PacketType_BYTE_ARRAY_DATA_REPLY) {

//            ByteArrayDataPacket data;
//			if (!data.ParseFromArray(ctrllinepacket.data().data(), ctrllinepacket.data().size())) {
//				log_info("Failed parse ByteArrayDataPacket");
//			}

////			log_info("Board ID:::" + QString::number((quint32)data.id()));
////			log_info("Board PORT:::" + QString::number((quint32)data.port()));
////			log_info(DebugUtils::printByteArray(QByteArray(data.data().data(), data.data().size())));

//			emit onBoardDataReceived( m_ktrIp, (quint32)data.id(), (quint32)data.port(), QByteArray(data.data().data(), data.data().size()) );
//		}

		buffer.remove(0, size_proto);
	}
}

void KTPSendNMEA::onConnectedSlot()
{
	//Subscribe on messages
	QByteArray exnglink;
	EXNGLinkPacket exngpacket;
	exngpacket.set_clientslistlink(false);
    exngpacket.set_alldatapackageslink(true);
	exngpacket.set_allrxtrylink(false);
	exngpacket.set_alltxtrylink(false);
	exngpacket.set_serversstatelink(false);
    exngpacket.set_gpslink(true);
    exngpacket.set_alldatapackageslink(true);
	exnglink.resize(exngpacket.ByteSize());
	exngpacket.SerializeToArray(exnglink.data(), exngpacket.ByteSize());
	sendToKtp(CtrlLinePacket_PacketType_EXNG_LINK_SET_OR_GET, exnglink);
}

double KTPSendNMEA::convertNMEAtoDecimal(const double& nmea)
{
	qint32 degress = nmea/100;
	double minutes = fmod(nmea, 100);
	return ((double)degress + (minutes/60));
}


void KTPSendNMEA::sendDataToKtp(QString ktrIp, uint id, uint port, const QByteArray& inputData) {

	if(m_ktrIp != ktrIp) {
		return;
	}

	ByteArrayDataPacket dataPacket;
	QByteArray dataToSend;

	dataPacket.set_id(id);
	dataPacket.set_port(port);
	dataPacket.set_data(inputData.data(), inputData.size());

	dataToSend.resize(dataPacket.ByteSize());
	dataPacket.SerializeToArray(dataToSend.data(), dataPacket.ByteSize());

	sendToKtp(CtrlLinePacket_PacketType_BYTE_ARRAY_DATA_SET_OR_GET, dataToSend);
}

void KTPSendNMEA::sendToKtp(quint32 type, const QByteArray& data) {
	m_packetNumber++;
	CtrlLinePacket ctrllinepacket;
	ctrllinepacket.set_type((CtrlLinePacket_PacketType)type);
	ctrllinepacket.set_data(data.data(), data.size());
	QByteArray ctrl;
	quint16 size_proto = ctrllinepacket.ByteSize();
	ctrl.resize(size_proto);
	ctrllinepacket.SerializeToArray(ctrl.data(), size_proto);

	QByteArray toSend;
	toSend.append(m_preambula);
	quint16 m_packetNumberB = qToBigEndian(m_packetNumber);
	quint16 size_protoB = qToBigEndian(size_proto);
	toSend.append(QByteArray((char*)&m_packetNumberB, sizeof(m_packetNumberB)));
	toSend.append(QByteArray((char*)&size_protoB, sizeof(size_protoB)));
	toSend.append(ctrl);
	BaseTcpClient::write(toSend);
}
