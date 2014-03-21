#include <QPointF>
#include <QDebug>

#include "BLAParser.h"

BLAParser::BLAParser(int id, IRouter *router)
{
	_id = id;
	_latitude = 0;
	_longitude = 0;
	_altitude = 0;
	_router = router;
	_subscriber = _router->get_subscriber();

	QString fileNameSpec = "logBPLA.txt";
	_fl = new QFile();

	_fl->setFileName(fileNameSpec);
	_fl->open(QIODevice::WriteOnly);
}

BLAParser::~BLAParser()
{
	_fl->close();
	delete _fl;
}

void BLAParser::parsing(QByteArray data)
{
	char f1 = data.at(0);
	char f2 = data.at(1);
	QString r1 = "A";
	QString r2 = "G";
	QString r3 = "N";
	_latitude = 0;
	_longitude = 0;

	if(!(f1 == r1.toAscii().at(0) && f2 == r2.toAscii().at(0)))
	{
		if(f2 == r3.toAscii().at(0))
		{
			///alt
			_altitude = (double)((((long)data.at(2+6)) << 8) | ((long)(data.at(2+7)) & 255));
		}
		return;
	}

	long latValue = 0;
	long lonValue = 0;
	long deg = 0;
	long min = 0;
	long minParts = 0;

	qDebug() << "parse data from KTR";
	for(int i = 0+2; i < 3+2; i++)
	{
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

	_latitude = (int) (lat * 100000);
	_latitude = lat;
	_longitude = (int) (lon * 100000);
	_longitude = lon;

	QPointF point;
	point.setX(_latitude);
	point.setY(_longitude);

	QByteArray *ba = new QByteArray();
	QDataStream ds(ba, QIODevice::ReadWrite);
//    ds << _id;
	ds << point;
	ds << _altitude;

	if(_fl->isOpen())
	{
		QString data;
		data.append(QDateTime::currentDateTime().toString("dd-MM-yyyy  hh-mm-ss.zzz"));
		data.append(" ");
		data.append(QString::number(_id));
		data.append(" ");
		data.append(QString::number(_latitude, 'f', 6));
		data.append(" ");
		data.append(QString::number(_longitude, 'f', 6));
		data.append(" ");
		data.append(QString::number(_altitude, 'f', 6));
		data.append("\n");
		_fl->write(data.toStdString().c_str());
		_fl->flush();
	}


	QSharedPointer<IMessage> msg(new Message(_id, KTR_BLA, ba));
	_subscriber->data_ready(KTR_BLA, msg);
}

void BLAParser::distruct()
{
	delete this;
}
