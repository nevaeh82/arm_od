#include <QPointF>
#include <QDebug>

#include "BLAParser2.h"

BLAParser2::BLAParser2(int id, IRouter *router)
{
	_id = id;
	_latitude = 0;
	_longitude = 0;
	_altitude = 0;
	_router = router;
	_subscriber = _router->get_subscriber();

	QString fileNameSpec = "logBPLA2.txt";
	_fl = new QFile();

	_fl->setFileName(fileNameSpec);
	_fl->open(QIODevice::WriteOnly);
}

BLAParser2::~BLAParser2()
{
	_fl->close();
	delete _fl;
}

void BLAParser2::parsing(QByteArray data)
{
	//$KTPGA,OBJ_ID=1044,2X,P0={42*38'32.14"|41*38'7.81"},P1={42*35'1.21"|41*50'53.51"},END
	QString r1 = "$KTPGA";

	QString data1(data.constData());

	QStringList dl1 = data1.split("$KTPGA,");
	QString ddt = dl1.at(1);
	if(!ddt.contains("END"))
	{
		return;
	}

	QStringList dl2 = ddt.split("END");
	QString final_string = dl2.at(0);

	QStringList dl3 = final_string.split(",");
//    qDebug() << "her";

	QString data2 = dl3.at(1);
	QString data3 = data2.left(data2.length() - 1);

	int count_points = data3.toInt();
	if(count_points > 2)
	{
		count_points = 1;
	}

	QVector<QPointF> vec_points;
	for(int i = 0; i < count_points; ++i)
	{
		QString f1 = dl3.at(i + 2);
		//P0={42*38'32.14"|41*38'7.81"}
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
//        qDebug() << lat_f;

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
//        qDebug() << lon_f;
		QPointF point(lat_f, lon_f);
		vec_points.push_back(point);
	}

	QByteArray *ba = new QByteArray();
	QDataStream ds(ba, QIODevice::ReadWrite);
//    ds << _id;
	ds << vec_points;
	double alt = 0;
	ds << alt;

	QSharedPointer<IMessage> msg(new Message(_id, KTR_BLA, ba));
	_subscriber->data_ready(KTR_BLA, msg);
}

void BLAParser2::distruct()
{
	delete this;
}
