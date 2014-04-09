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
//	log_debug(answer);
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
	//! получение координат кавычек и запятых и занесение их в список listBr
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
			listBr << br;
			c = 0;
			inList = false;
		}
	}

	//! парсинг данных
	for (int i = 0; i < listBr.size(); i++) {
		int x1;
		int x2;
		QString latitude;
		QString longinude;
		QString height;
		QString speed;
		QString course;
		QString namePlane;

		AISZone zone = m_zoneManager->getLastZone();

		//! получение широты расположения самолета
		x1 = listBr[i].comma[0] + 1;
		x2 = listBr[i].comma[1];
		latitude = data.mid(x1, x2 - x1);

		if (zone.latitudeMax >= latitude.toDouble() && latitude.toDouble() >= zone.latitudeMin)
		{
			//! получение долготы расположения самолета
			x1 = listBr[i].comma[1] + 1;
			x2 = listBr[i].comma[2];
			longinude = data.mid(x1, x2 - x1);

			if (zone.longitudeMin <= longinude.toDouble() && longinude.toDouble() <= zone.longitudeMax) {
				//! получение курса самолета
				x1 = listBr[i].comma[2] + 1;
				x2 = listBr[i].comma[3];
				course = data.mid(x1, x2 - x1);

				//! получение высоты расположения самолета
				x1 = listBr[i].comma[3] + 1;
				x2 = listBr[i].comma[4];
				height = data.mid(x1, x2 - x1);
				double h = height.toDouble() * 0.3048;
				height = QString().number(h);

				//! получение скорости самолета
				x1 = listBr[i].comma[4] + 1;
				x2 = listBr[i].comma[5];
				speed = data.mid(x1, x2 - x1);

				//! получение названия самолета
				x1 = listBr[i].comma[15] + 2;
				x2 = listBr[i].comma[16] - 1;
				namePlane = data.mid(x1, x2 - x1);

				//! внесение полученных данных в список listDataFly
				DataFly data;
				data.namePlane = namePlane;
				data.latitute = latitude;
				data.longinude = longinude;
				data.speed = speed;
				data.height = height;
				data.course = course;
				QVector<QString> vec_str;
				vec_str.push_back(namePlane);
				vec_str.push_back(longinude);
				vec_str.push_back(latitude);
				vec_str.push_back(speed);
				vec_str.push_back(height);
				vec_str.push_back(course);


				map1.insert(i, vec_str);



				listDataFly << data;
			}
		}
	}
	listBr.clear();

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	/// TODO: recheck id
//	dataStream << _id;
	dataStream << map1;

	listDataFly.clear();
	return MessageSP(new Message<QByteArray>(TCP_AIS_ANSWER_DATA, dataToSend));
}
