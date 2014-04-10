#include "TcpAISCoder.h"

TcpAISCoder::TcpAISCoder(ITcpAISZoneManager* zoneManager, QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
	m_zoneManager = zoneManager;
}

TcpAISCoder::~TcpAISCoder()
{
}

MessageSP TcpAISCoder::encode(const QByteArray& data)
{
	QByteArray inputData = data;
	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);
	QString answer;
	inputDataStream >> answer;

	if (answer.size() == 0) {
		log_debug("Empty answer");
		return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, QByteArray()));
	}
	log_debug("Answer is not empty");

	return filterData(answer);
}

QByteArray TcpAISCoder::decode(const MessageSP message)
{
	return message->data();
}

MessageSP TcpAISCoder::filterData(const QString& data)
{
	int c = 0;
	bool inList = false;
	QMap<int, QVector<QString> > map1;
	/// Getting brackets and commas and adding them to listBr
	for (int i = 0; i < data.size(); i++) {
		Bracketed br;
		if (data.mid(i, 1) == "[") {
			br.br1 = i;
			inList = true;
		}
		if ((data.mid(i, 1) == ",") && (inList == true)) {
			br.comma[c] = i;
			c++;
		}
		if (data.mid(i, 1) == "]") {
			br.br2 = i;
			m_listBr << br;
			c = 0;
			inList = false;
		}
	}

	/// Parsing
	for (int i = 0; i < m_listBr.size(); i++) {
		int x1;
		int x2;
		QString latitude;
		QString longinude;
		QString altitude;
		QString speed;
		QString course;
		QString namePlane;

		AISZone zone = m_zoneManager->getLastZone();

		/// Getting latitude
		x1 = m_listBr[i].comma[0] + 1;
		x2 = m_listBr[i].comma[1];
		latitude = data.mid(x1, x2 - x1);

		if (zone.latitudeMax >= latitude.toDouble() && latitude.toDouble() >= zone.latitudeMin)
		{
			/// Getting longitude
			x1 = m_listBr[i].comma[1] + 1;
			x2 = m_listBr[i].comma[2];
			longinude = data.mid(x1, x2 - x1);

			if (zone.longitudeMin <= longinude.toDouble() && longinude.toDouble() <= zone.longitudeMax) {
				/// Getting course
				x1 = m_listBr[i].comma[2] + 1;
				x2 = m_listBr[i].comma[3];
				course = data.mid(x1, x2 - x1);

				/// Getting altitude
				x1 = m_listBr[i].comma[3] + 1;
				x2 = m_listBr[i].comma[4];
				altitude = data.mid(x1, x2 - x1);
				double h = altitude.toDouble() * 0.3048;
				altitude = QString().number(h);

				/// Getting plane speed
				x1 = m_listBr[i].comma[4] + 1;
				x2 = m_listBr[i].comma[5];
				speed = data.mid(x1, x2 - x1);

				// Getting plane name
				x1 = m_listBr[i].comma[15] + 2;
				x2 = m_listBr[i].comma[16] - 1;
				namePlane = data.mid(x1, x2 - x1);

				QVector<QString> vec_str;
				vec_str.push_back(namePlane);
				vec_str.push_back(longinude);
				vec_str.push_back(latitude);
				vec_str.push_back(speed);
				vec_str.push_back(altitude);
				vec_str.push_back(course);

				map1.insert(i, vec_str);

			}
		}
	}
	m_listBr.clear();

	log_debug(QString("Plane count = %1").arg(map1.count()));

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << map1;
	return MessageSP(new Message<QByteArray>(TCP_AIS_ANSWER_DATA, dataToSend));
}
