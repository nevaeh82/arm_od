#include "TcpKTRCoder.h"

TcpKTRCoder::TcpKTRCoder(QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
	m_altitude = 0;

	QString fileName = "./logBPLA.txt";
	m_logFile.setFileName(fileName);

	if(!m_logFile.open(QIODevice::WriteOnly)) {
		debug(QString("Cannot open file: %1").arg(fileName));
	}
}

TcpKTRCoder::~TcpKTRCoder()
{
	m_logFile.close();
}

MessageSP TcpKTRCoder::encode(const QByteArray& data)
{
	debug(QString("Parse data from KTR %1").arg(QString::fromAscii(data.constData(), data.size())));

	//parseLocationFromKTR

	//$KTPGA,OBJ_ID=1044,2X,P0={42*38'32.14"|41*38'7.81"},P1={42*35'1.21"|41*50'53.51"},END

	QString inputDataAsString(data.constData());

	QStringList dl1 = inputDataAsString.split("$KTPGA,");
	if (dl1.size() > 1) {

		QString ddt = dl1.at(1);
		if (ddt.contains("END")) {
			debug("CASE ONE");
			return parseLocationFromKTR(data);
		}
	}

	//parseLocationFromBoard

	char f1 = data.at(0);
	char f2 = data.at(1);
	QString r1 = "A";
	QString r2 = "G";
	QString r3 = "N";

	if ((f1 == r1.toAscii().at(0) && f2 == r2.toAscii().at(0))) {
		debug("CASE TWO");
		return parseLocationFromBoard(data);
	}
	else if (f2 == r3.toAscii().at(0)) {
		m_altitude = (double)((((long)data.at(2+6)) << 8) | ((long)(data.at(2+7)) & 255));
	}

	//parseBoardList

	if (data.mid(0, 12) == "control link") {
		debug("CASE THREE");
		return parseBoardList(data);
	}


	return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, QByteArray()));

}

QByteArray TcpKTRCoder::decode(const MessageSP message)
{
	/**
	 * TCP_KTR_REQUEST_GET_BOARD_LIST
	 * "user link command ctrl" as Ascii in QByteArray
	 *
	 * TCP_KTR_REQUEST_COMMAND_TO_BPLA
	 *
	 * case 1:
	 * "user link command b" + QString::number(_id) + "d" +QString::number(_dev);
	 *
	 * case 2:
	 * "user link command b" + QString::number(_id) + "d" +QString::number(622);
	 **/

	QString messageType = message->type();

	QString command = "user link command ";
	QByteArray dataToSend;

	if (messageType == TCP_KTR_REQUEST_GET_BOARD_LIST) {
		command += "ctrl";
	} else if (messageType == TCP_KTR_REQUEST_COMMAND_TO_BPLA) {

		QByteArray inputByteArray = message->data();
		QDataStream inputDataStream(&inputByteArray, QIODevice::ReadOnly);

		quint16 id = 0;
		quint32 dev = 0;

		inputDataStream >> id >> dev;

		command += "b" + QString::number(id) + "d" + QString::number(dev);
	}

	dataToSend.append(command.toAscii());
	debug(command);
	return dataToSend;
}

MessageSP TcpKTRCoder::parseLocationFromBoard(const QByteArray& data)
{
	double latitude = 0;
	double longitude = 0;

	long latValue = 0;
	long lonValue = 0;
	long deg = 0;
	long min = 0;
	long minParts = 0;

	for (int i = 0+2; i < 3+2; i++) {
		latValue <<= 8;
		latValue |= ((long)data.at(i)) & 255;
		lonValue <<= 8;
		lonValue |= ((long)data.at(i+3)) & 255;
	}

	deg = latValue / 100000;
	min = latValue - (deg * 100000);
	min = min / 1000;
	minParts = latValue - (deg * 100000) - (min * 1000);

	double lat = (double) minParts / (double) 1000;
	lat += min;
	lat /= 60;
	lat += deg;

	deg = lonValue / 100000;
	min = lonValue - (deg * 100000);
	min = min / 1000;
	minParts = lonValue - (deg * 100000) - (min * 1000);

	double lon = (double) minParts / (double) 1000;
	lon += min;
	lon /= 60;
	lon += deg;

//	latitude = (int) (lat * 100000);
	latitude = lat;
//	longitude = (int) (lon * 100000);
	longitude = lon;

	QPointF point;
	point.setX(latitude);
	point.setY(longitude);


	if(m_logFile.isOpen()) {
		QString dataString;
		dataString.append(QDateTime::currentDateTime().toString("dd-MM-yyyy  hh-mm-ss.zzz"));
		/// TODO : recheck
//			dataString.append(" ");
//		dataString.append(QString::number(_id));
		dataString.append(" ");
		dataString.append(QString::number(latitude, 'f', 6));
		dataString.append(" ");
		dataString.append(QString::number(longitude, 'f', 6));
		dataString.append(" ");
		dataString.append(QString::number(m_altitude, 'f', 6));
		dataString.append("\n");
		m_logFile.write(dataString.toStdString().c_str());
		m_logFile.flush();
	}

	UAVPositionData positionData;
	positionData.latitude = latitude;
	positionData.longitude = longitude;
	positionData.altitude = m_altitude;

	QVector<UAVPositionData> positionDataVector;
	positionDataVector.push_back(positionData);

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	dataStream << positionDataVector;

	return MessageSP(new Message<QByteArray>(TCP_KTR_ANSWER_BPLA, dataToSend));
}

MessageSP TcpKTRCoder::parseLocationFromKTR(const QByteArray& data)
{
	QString inputDataAsString(data.constData());

	QStringList dl1 = inputDataAsString.split("$KTPGA,");
	QString ddt = dl1.at(1);

	QStringList dl2 = ddt.split("END");
	QString final_string = dl2.at(0);

	QStringList dl3 = final_string.split(",");

	QString data2 = dl3.at(1);
	QString data3 = data2.left(data2.length() - 1);

	int count_points = data3.toInt();
	if (count_points > 2) {
		count_points = 1;
	}

	QVector<UAVPositionData> positionDataVector;
	for (int i = 0; i < count_points; ++i) {
		QString f1 = dl3.at(i + 2);
		QStringList l33 = f1.split("{");

		f1 = l33.at(1);
		f1.remove("}");
		l33 = f1.split("|");

		QString lat = l33.at(0);
		QStringList tmp1 = lat.split("*");
		double deg = tmp1.at(0).toDouble();
		QStringList tmp2 = tmp1.at(1).split("'");
		double min = tmp2.at(0).toDouble();
		QStringList tmp3 = tmp2.at(1).split("'");
		QString ss = tmp3.at(0);
		ss.remove("\"");
		double sec = ss.toDouble();

		double lat_f = sec/60;
		lat_f += min;
		lat_f /= 60;
		lat_f += deg;

		QString lon = l33.at(1);
		QStringList tmp11 = lon.split("*");
		double deg1 = tmp11.at(0).toDouble();
		QStringList tmp21 = tmp11.at(1).split("'");
		double min1 = tmp21.at(0).toDouble();
		QStringList tmp31 = tmp21.at(1).split("'");
		QString ss1 = tmp31.at(0);
		ss.remove("\"");
		double sec1 = ss1.toDouble();

		double lon_f = sec1/60;
		lon_f += min1;
		lon_f /= 60;
		lon_f += deg1;

		UAVPositionData positionData;
		positionData.latitude = lat_f;
		positionData.longitude = lon_f;

		positionDataVector.push_back(positionData);
	}

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	dataStream << positionDataVector;

	return MessageSP(new Message<QByteArray>(TCP_KTR_ANSWER_BPLA, dataToSend));
}

MessageSP TcpKTRCoder::parseBoardList(const QByteArray& data)
{

	QByteArray inputData = data;
	inputData.remove(0, 12);

	int count_bla = inputData.at(0);
	QList<quint16> identificators;

	for (int i = 0; i < count_bla; i++) {
		quint16 id = ((quint8)inputData.at(3 + i*24) << 8) + (quint8)inputData.at(2 + i*24);
		identificators.append(id);
		debug(QString("ID = %1").arg(QString::number(id)));
	}

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << identificators;

	return MessageSP(new Message<QByteArray>(TCP_KTR_ANSWER_BOARD_LIST, dataToSend));
}
