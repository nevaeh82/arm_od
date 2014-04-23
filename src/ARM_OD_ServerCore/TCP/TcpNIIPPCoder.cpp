#include "TcpNIIPPCoder.h"

TcpNIIPPCoder::TcpNIIPPCoder(QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
}

TcpNIIPPCoder::~TcpNIIPPCoder()
{
}

MessageSP TcpNIIPPCoder::encode(const QByteArray& data)
{
	QString str(data);
	QStringList list = str.split(',');

	QDateTime dt;
	dt.fromString(list[0], "ddMMyy");

	QTime time;
	time.fromString(list[1], "HHmmss");

	int mode = list[2].toInt();

	QString lat_str = list[3];
	QString lat_sec_str = lat_str.right(2);

	int sec_int = lat_sec_str.toInt();
	double sec = (double)sec_int/3600;

	QString lat_min_str_temp = lat_str.right(4);
	QString lat_min_str = lat_min_str_temp.left(2);

	int min_int = lat_min_str.toInt();
	double min = (double)min_int/60;

	QString lat_grad = lat_str.left(lat_str.length() - 4);

	int grad_int = lat_grad.toInt();
	double grad = (double)grad_int + min + sec;

	QPointF point;
	point.setX(grad);

	QString NS = list[4];

	QString lon_str = list[5];
	QString lon_sec_str = lon_str.right(2);
	sec_int = lon_sec_str.toInt();
	sec = (double)sec_int/3600;

	QString lon_min_str_temp = lon_str.right(4);
	QString lon_min_str = lon_min_str_temp.left(2);
	min_int = lon_min_str.toInt();
	min = (double)min_int/60;

	QString lon_grad = lon_str.left(lon_str.length() - 4);
	grad_int = lon_grad.toInt();
	grad = (double)grad_int + min + sec;
	point.setY(grad);

	QString EW = list[6];

	int alt = list[7].toInt();
	int zone = list[8].toInt();
	int course = list[9].toInt();
	int angle = list[10].toInt();

	QByteArray dataToSend;
	QDataStream ds(&dataToSend, QIODevice::ReadWrite);

	/// TODO : recheck _id
    ds << 100;//	ds << _id;

	ds << dt;
	ds << time;
	ds << mode;
	ds << point;
	ds << NS;
	ds << EW;
	ds << alt;
	ds << zone;
	ds << course;
	ds << angle;

	log_debug(QString("MMMMMMMMMMMMMMMMMMMMMM %1 %2 %3").arg(course).arg(angle).arg(mode));
	return MessageSP(new Message<QByteArray>(TCP_NIIPP_ANSWER, dataToSend));

//	QSharedPointer<IMessageOld> msg(new MessageOld(_id, NIIPP_ANSWER, ba));
//	_subscriber->data_ready(NIIPP_ANSWER, msg);
//	qDebug() << "MMMMMMMMMMMMMMMMMMMMMM" << course << angle << mode;

}

QByteArray TcpNIIPPCoder::decode(const MessageSP message)
{
	if (message->type() != TCP_NIIPP_REQUEST_PBLA) {
		return QByteArray();
	}

	QByteArray inputData = message->data();
	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);

	QDate date;
	QTime time;
	int id;

	inputDataStream >> id;
	inputDataStream >> date;
	inputDataStream >> time;

	QString comm;
	inputDataStream >> comm;

	QPointF point;
	inputDataStream >> point;
	QString NS;
	inputDataStream >> NS;

	QString EW;

	inputDataStream >> EW;
	int alt;
	inputDataStream >> alt;

	int velocity_direction = 0;
	inputDataStream >> velocity_direction;
	int zone;
	inputDataStream >> zone;

	QPointF point2;
	inputDataStream >> point2;
	QString NS2;
	inputDataStream >> NS2;
	QString EW2;
	inputDataStream >> EW2;

	QStringList list;
	list.append(date.toString("ddMMyy"));
	list.append(time.toString("HHmmss"));
	list.append(comm);

	QString lat1;
	if(point.x() == 0)
	{
		lat1 = "000000";
	}
	else
	{
		QString lat = QString::number(point.x(), 'f', 6);
		QStringList list_lat1 = lat.split('.');

		QString lat_deg = list_lat1[0];//lat.left(2);
		double lat_min1 = lat.toDouble();
		double lat_min2 = lat_deg.toDouble();
		lat_min1 = lat_min1 - lat_min2;
		double lat_min_final = lat_min1 * 60;
		int lat_min_final1 = (int)lat_min_final;
		QString lat_min = QString::number(lat_min_final1);
		if(lat_min.length() < 2)
		{
			lat_min.insert(0, "0");
		}

		double lat_sec1 = lat_min_final - (double)lat_min_final1;
		lat_sec1 = lat_sec1 * 60;
		int lat_sec2 = lat_sec1;

		QString lat_sec = QString::number(lat_sec2);
		if(lat_sec.length() < 2)
		{
			lat_sec.insert(0, "0");
		}

		lat1 = lat_deg + lat_min + lat_sec;
	}

	list.append(lat1);
	list.append(NS);


	QString lon1;
	if(point.y() == 0)
	{
		lon1 = "000000";
	}
	else
	{
		QString lon = QString::number(point.y(), 'f', 6);
		QStringList list_lon1 = lon.split('.');

		QString lon_deg = list_lon1[0];//lat.left(2);
		double lon_min1 = lon.toDouble();
		double lon_min2 = lon_deg.toDouble();
		lon_min1 = lon_min1 - lon_min2;
		double lon_min_final = lon_min1 * 60;
		int lon_min_final1 = (int)lon_min_final;
		QString lon_min = QString::number(lon_min_final1);
		if(lon_min.length() < 2)
		{
			lon_min.insert(0, "0");
		}

		double lon_sec1 = lon_min_final - (double)lon_min_final1;
		lon_sec1 = lon_sec1 * 60;
		int lon_sec2 = lon_sec1;

		QString lon_sec = QString::number(lon_sec2);
		if(lon_sec.length() < 2)
		{
			lon_sec.insert(0, "0");
		}

		lon1 = lon_deg + lon_min + lon_sec;
	}

	list.append(lon1);
	list.append(EW);

	int alt1 = (int)alt;
	int bear = (int)velocity_direction;

	list.append(QString::number(alt1));
	list.append(QString::number(bear));
	list.append(QString::number(zone));

	QString lat1_point2;
	if(point2.x() == 0)
	{
		lat1_point2 = "000000";
	}
	else
	{
		QString lat_point2 = QString::number(point.x(), 'f', 6);
		QStringList list_lat1_point2 = lat_point2.split('.');

		QString lat_deg_point2 = list_lat1_point2[0];//lat.left(2);
		double lat_min1_point2 = lat_point2.toDouble();
		double lat_min2_point2 = lat_deg_point2.toDouble();
		lat_min1_point2 = lat_min1_point2 - lat_min2_point2;
		double lat_min_final_point2 = lat_min1_point2 * 60;
		int lat_min_final1_point2 = (int)lat_min_final_point2;
		QString lat_min_point2 = QString::number(lat_min_final1_point2);
		if(lat_min_point2.length() < 2)
		{
			lat_min_point2.insert(0, "0");
		}

		double lat_sec1_point2 = lat_min_final_point2 - (double)lat_min_final1_point2;
		lat_sec1_point2 = lat_sec1_point2 * 60;
		int lat_sec2_point2 = lat_sec1_point2;

		QString lat_sec_point2 = QString::number(lat_sec2_point2);
		if(lat_sec_point2.length() < 2)
		{
			lat_sec_point2.insert(0, "0");
		}

		lat1_point2 = lat_deg_point2 + lat_min_point2 + lat_sec_point2;
	}

	list.append(lat1_point2);
	list.append(NS2);

	QString lon1_point2;
	if(point2.y() == 0)
	{
		lon1_point2 = "000000";

	}
	else
	{
		QString lon_point2 = QString::number(point.y(), 'f', 6);
		QStringList list_lon1_point2 = lon_point2.split('.');

		QString lon_deg_point2 = list_lon1_point2[0];//lat.left(2);
		double lon_min1_point2 = lon_point2.toDouble();
		double lon_min2_point2 = lon_deg_point2.toDouble();
		lon_min1_point2 = lon_min1_point2 - lon_min2_point2;
		double lon_min_final_point2 = lon_min1_point2 * 60;
		int lon_min_final1_point2 = (int)lon_min_final_point2;
		QString lon_min_point2 = QString::number(lon_min_final1_point2);
		if(lon_min_point2.length() < 2)
		{
			lon_min_point2.insert(0, "0");
		}

		double lon_sec1_point2 = lon_min_final_point2 - (double)lon_min_final1_point2;
		lon_sec1_point2 = lon_sec1_point2 * 60;
		int lon_sec2_point2 = lon_sec1_point2;

		QString lon_sec_point2 = QString::number(lon_sec2_point2);
		if(lon_sec_point2.length() < 2)
		{
			lon_sec_point2.insert(0, "0");
		}

		lon1_point2 = lon_deg_point2 + lon_min_point2 + lon_sec_point2;
	}

	list.append(lon1_point2);
	list.append(EW2);

	return prepareDataToSend(list);
}

QByteArray TcpNIIPPCoder::prepareDataToSend(const QStringList& list)
{
	QString str = list.join(",");
	quint8 _crc = crc(str.toLocal8Bit());

	str.append(",*");
	str.append(QString::number(_crc));
	str.append("\\r\\n");

	return str.toLocal8Bit();
}

quint8 TcpNIIPPCoder::crc(const QByteArray& data)
{
	quint8 crc = 0;
	for (int i = 0; i < data.length(); i++) {
		crc ^= data.at(i);
	}
	return crc;
}
