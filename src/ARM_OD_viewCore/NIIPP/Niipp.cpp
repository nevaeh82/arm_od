#include "Niipp.h"

const double m_zone[24] = {1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5,
					   5, 6, 7, 8, 9, 10, 11, 12, 14, 16,
					   18, 20, 22, 24, 28, 30};
const double m_zoneDir[28] = {2.5, 3, 4, 5,
					   6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20,
					   22, 26, 29, 33, 37, 41, 47, 52, 57, 62, 68, 72, 76};

Niipp::Niipp(int id, QString name, QPointF latlon, MapController* mapController, ITabManager* parentTab)
	: m_numberCommand("0200")
	, m_antenaType(0)
	, m_widthAngle(25)
	, m_modeCurrentIndex(disabled)
	, m_type(0)
	, m_mode(0)
	, m_antenaIndex(0)
	, m_sbPowerValue(0)
{
	m_id = id;
	m_name = name;
	m_latlon = latlon;
	m_parentTab = parentTab;
	m_mapController = mapController;
	m_radiusSector = 0;
	m_radiusCircle = 0;
	m_angel = 0;
}

void Niipp::create()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);

	ds << m_name;
	ds << m_latlon;
	ds << m_widthAngle;

	m_mapController->getMapClient(1)->updateNiippPowerSector(m_id, 0, m_angel, ba);
}

void Niipp::setAngel(double value)
{
	m_angel = value;
}

void Niipp::stopCommad()
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);

	QDateTime dt = QDateTime::currentDateTime();
	QDate date = dt.date();
	ds << m_id;
	ds << date;
	QTime time = dt.time();
	ds << time;

	QString status = "0300";
	ds << status;

	QPointF point1;
	point1.setX(0);
	point1.setY(0);
	ds << point1;

	QString s1 = "N";
	ds << s1;
	s1 = "E";
	ds << s1;
	int alt = 0;
	ds << alt;
	int velocity_dir = 0;
	ds << velocity_dir;
	int zone = 0;
	ds << zone;

	QPointF point2;
	point2.setX(0);
	point2.setY(0);
	ds << point2;
	s1 = "N";
	ds << s1;
	s1 = "E";
	ds << s1;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_NIIPP_BPLA, ba);
	m_parentTab->send_data(0, msg);
}

void Niipp::enableComplex(bool state)
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);

	QString status;

	if(state)
	{
		status = "0000";
	}
	else
	{
		status = "0001";
	}

	QDateTime dt = QDateTime::currentDateTime();
	QDate date = dt.date();
	ds << m_id;
	ds << date;
	QTime time = dt.time();
	ds << time;
	ds << status;

	QPointF point1;
	point1.setX(0);
	point1.setY(0);
	ds << point1;

	QString s1 = "N";
	ds << s1;
	s1 = "E";
	ds << s1;

	int alt = 0;
	ds << alt;
	int velocity_dir = 0;
	ds << velocity_dir;
	int zone = 0;
	ds << zone;

	QPointF point2;
	point2.setX(0);
	point2.setY(0);
	ds << point2;
	s1 = "N";
	ds << s1;
	s1 = "E";
	ds << s1;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_NIIPP_BPLA, ba);
	m_parentTab->send_data(0, msg);
}

void Niipp::clear()
{
	m_pointUvodeNiipp.setX(0);
	m_pointUvodeNiipp.setY(0);

	m_mapController->getMapClient(1)->removePointUvoda();
}

void Niipp::changeValuePower(int value)
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);

	ds << m_name;
	ds << m_latlon;
	ds << m_widthAngle;

	if(m_antenaType == 0)
	{
		m_mapController->getMapClient(1)->updateNiippPowerSector(m_id, m_zoneDir[value], m_angel, ba);
		m_radiusSector = m_zoneDir[value];
	}
	if(m_antenaType == 1)
	{
		m_mapController->getMapClient(1)->updateNiippPowerCicle(m_id, m_zone[value], ba);
		m_radiusCircle = m_zone[value];
	}
}

void Niipp::setAntennaType(int value)
{
	m_antenaType = value;

	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);

	ds << m_name;
	ds << m_latlon;
	ds << m_widthAngle;

	if(m_antenaType == 1)
	{
		m_mapController->getMapClient(1)->updateNiippPowerCicle(m_id, m_radiusCircle, ba);
	}
	else
	{
		m_mapController->getMapClient(1)->updateNiippPowerSector(m_id, m_radiusSector, m_angel, ba);
	}
}

void Niipp::sendEvil(const QPointF& point, const QPointF& diversionPoint, double alt, double bearing)
{
	Q_UNUSED( diversionPoint );

	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);
	QDateTime dt = QDateTime::currentDateTime();

	ds << m_id;
	ds << dt.date();
	ds << dt.time();

	int index = m_antenaIndex;
	QString str_temp;
	if(index == 1)
	{
		str_temp =  "0100";
	}
	else
	{
		switch(m_mode)
		{
		case 0:
			str_temp = "0201";
			break;
		case 1:
			str_temp = "0202";
			break;
		case 2:
			str_temp = "0204";
			break;
		default:
			str_temp = "0200";
			break;
		}
	}

	ds << str_temp;

	ds << point;
	QString NS = "N";
	ds << NS;

	QString EW = "E";

	ds << EW;

	int alt1 = (int)alt;
	ds << alt1;

	int bearing1 = (int)bearing;

	ds << bearing1;
	int zone = m_sbPowerValue;
	ds << zone;

	ds << m_pointUvodeNiipp;
	QString NS2 = "N";
	ds << NS2;
	QString EW2 = "E";
	ds << EW2;

	CommandMessage* msg = new CommandMessage(COMMAND_SET_NIIPP_BPLA, ba);
	m_parentTab->send_data(0, msg);
}

int Niipp::getId()
{
	return m_id;
}

int Niipp::getAntenaType()
{
	return m_antenaType;
}

double Niipp::getRadiusCircle()
{
	return m_radiusCircle;
}

double Niipp::getRadiusSector()
{
	return m_radiusSector;
}

Niipp::WorkMode Niipp::getModeCurrentIndex()
{
	return m_modeCurrentIndex;
}

QByteArray Niipp::encode(QStringList list)
{
	QString str = list.join(",");
	QByteArray output = str.toLocal8Bit();
	quint8 crc = 0;
	for(int i = 0; i < output.length(); i++)\
	{
		crc ^= output.at(i);
	}
	str.append(",*");
	str.append(QString::number(crc));
	str.append("\\r\\n");

	QByteArray ba_out;
	QDataStream ds(&ba_out, QIODevice::WriteOnly);
	ds << m_id;
	ds << str;
	return ba_out;
}

void Niipp::changeMode(int value)
{
	m_mode = value;
}

void Niipp::setMode(int value)
{
	switch (value) {
	case -1:
		m_modeCurrentIndex = disabled;
		break;
	case 0:
		m_modeCurrentIndex = radiation;
		break;
	case 1:
		m_modeCurrentIndex = adduction;
		break;
	case 2:
		m_modeCurrentIndex = roundView;
		break;
	default:
		m_modeCurrentIndex = radiation;
		break;
	}
}

void Niipp::setAntenaIndex(int value)
{
	m_antenaIndex = value;
}

void Niipp::setSBpowerValue(int value)
{
	m_sbPowerValue = value;
}

void Niipp::setPoint(const QPointF& coord)
{
	 m_pointUvodeNiipp = coord;
}


