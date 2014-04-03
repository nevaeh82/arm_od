#include "ZInterception.h"

ZInterception::ZInterception(IMapClient* client)
{
	_client = client;
	connect(this, SIGNAL(signalSet(int,int,QByteArray,QByteArray)), this, SLOT(_slot_set(int,int,QByteArray,QByteArray)));
}

ZInterception::~ZInterception()
{
	emit finished();
}

void ZInterception::set(int bla, int bpla, QByteArray per, QByteArray target)
{
	emit signalSet(bla, bpla, per, target);

}

void ZInterception::_slot_set(int bla, int bpla, QByteArray per, QByteArray target)
{
	_bla = bla;
	_bpla = bpla;
	QDataStream ds(&target, QIODevice::ReadOnly);
	int time;
	ds >> time;
	int state;
	ds >> state;
	QPointF sko;
	ds >> sko;
	QVector<QPointF> track;
	ds >> track;
	double speed;
	ds >> speed;
	double alt;
	ds >> alt;
	double bearing;
	ds >> bearing;


	QDataStream ds1(&per, QIODevice::ReadOnly);
	QPointF point1;
	ds1 >> point1;
	double alt1;
	ds1 >> alt1;
	double speed1;
	ds1 >> speed1;
	double course1;
	ds1 >> course1;
	int state1;
	ds1 >> state1;

	getIntcData(point1, alt1, 30, 5);
	getAimData(track.at(track.size()-1), alt, speed, bearing);

	mainProcessing();
}

void ZInterception::getIntcData(QPointF aCoordCurrentIntc, float aHgtCurrentIntc, float aMaxSpd, float aMaxHgtSpd)
{
//    qDebug()<<"Заполнены данные перехватчика";
	mCoordCurrentIntc=aCoordCurrentIntc; //qDebug()<<"Координаты "<<aCoordCurrentIntc.x()<<" "<<aCoordCurrentIntc.y();
	mHgtCurrentIntc=aHgtCurrentIntc; //qDebug()<<"Высота "<<aHgtCurrentIntc;
	mMaxSpd=aMaxSpd; //qDebug()<<"Скорость "<<aMaxSpd;
	mMaxHgtSpd=aMaxHgtSpd; //qDebug()<<"Скорость набора высоты"<<aMaxHgtSpd;

}

void ZInterception::getAimData(QPointF aCoordCurrentAim, float aHgtCurrentAim, float aSpdAim, float aCourseAim)
{
	//qDebug()<<"Заполнены данные Цели";
	mCoordCurrentAim=aCoordCurrentAim; //qDebug()<<"Координаты "<<aCoordCurrentAim.x()<<" "<<aCoordCurrentAim.y();
	mHgtCurrentAim=aHgtCurrentAim; //qDebug()<<"Высота "<<aHgtCurrentAim;
	mCurrentAimSpd=aSpdAim; //qDebug()<<"Скорость "<<aSpdAim;
	mCurrentAimCourse=aCourseAim; //qDebug()<<"Курс "<<aCourseAim;

}

void ZInterception::mainProcessing()
{
	//qDebug()<<"Пошла основная обработка";

	uint aAllTime=60*60;  //Час на перехват

	QPointF aTempInterceptionCoord;
	QPointF aDecartCoordAim=QPointF(0,0);               //Декартовы коориднаты цели
	QPointF aDecartCoordIntc;                           //Декартовы координаты перехватчика
	geogrToDecart(mCoordCurrentIntc, aDecartCoordIntc, mCoordCurrentAim);

	float aTempDistance;                                //Расстояние, которое пролетела цель
	float aTempTimeToIntercept;                         //Время, необходимое перехватчику, чтобы долететь до точки перехвата
	float aTempTimeToHgtIntercept;                      //Время, необходимое перехватчику, чтобы набрать высоту точки перехвата
	//Расчет координат каждой точки цели в течение часа
	for (uint i=0; i < aAllTime; i++)
	{
		//qDebug()<<"I в основной обработке "<<i;

		//Текущая координата цели
		getNextCoord(aDecartCoordAim, mCurrentAimCourse, mCurrentAimSpd, i, aTempInterceptionCoord);
		if (mCurrentAimSpd<0.1) aTempInterceptionCoord=aDecartCoordAim;

		//Расстояние от текущей координаты цели до перехватчика
		aTempDistance=getDistance(aTempInterceptionCoord, aDecartCoordIntc);
		//qDebug()<<" aTempDistance="<<aTempDistance;

		//Определение времени, за которое сюда долетит перехватчик
		aTempTimeToIntercept=aTempDistance/mMaxSpd;
		//qDebug()<<" aTempTimeToIntercept="<<aTempTimeToIntercept;

		//Определение времени, за которое перехватчик наберет высоту
		aTempTimeToHgtIntercept=(mHgtCurrentAim+1000-mHgtCurrentIntc)/mMaxHgtSpd;

		//Определение условия выполнения перехвата
		if (i>aTempTimeToIntercept && i>aTempTimeToHgtIntercept)
		{
			//Перехват возможен, определяем курс, скорость, высоту перехватчика
			QPointF aIntcCoords=aTempInterceptionCoord;               //Координаты перехвата
			float aCurrentAngle;                                      //Необходимый курсовой угол для перехватчика
			QPointF aXYSpeed;
			getAngle(aDecartCoordIntc, aIntcCoords, mMaxSpd, aXYSpeed, aCurrentAngle);

			float aCurrentHgtInt=i*mMaxHgtSpd;
			float aRadius=aCurrentHgtInt-mHgtCurrentAim;

			//Отправка сигнала с данными о точке перехвата
			decartToGeogr(aTempInterceptionCoord, aIntcCoords, mCoordCurrentAim);

//            emit signal_sendInterceptionPoint(aIntcCoords, aCurrentHgtInt, aRadius, i, aCurrentAngle, mMaxSpd);
			_client->addPerehvatPoint( _bla, _bpla, aIntcCoords, aCurrentHgtInt, aRadius, i, aCurrentAngle, mMaxSpd );
			//Присвоение времени для выхода из цикла
			i=aAllTime+10;
		}
	}
}


void ZInterception::geogrToDecart(QPointF aGeogrCoord,QPointF &lDecartCoord, QPointF aCentrCoord)
{
	float dY, dX, popravka;
	float pi=3.1415926535897932385F;
	float ERadius=6366197.724F;

	popravka=cos(aCentrCoord.x()*pi/180);

	dY=(aGeogrCoord.x()-aCentrCoord.x())*pi*ERadius/180;
	dX=(aGeogrCoord.y()-aCentrCoord.y())*pi*ERadius/180;
	lDecartCoord=QPointF(dX*popravka, dY);
}

void ZInterception::decartToGeogr(QPointF aDecartCoord, QPointF &lGeogrCoord, QPointF aCentrCoord)
{
	float pi=3.1415926535897932385F;
	float ERadius=6366197.724F;

	lGeogrCoord.setX(aCentrCoord.x() + aDecartCoord.y()*180/pi/ERadius);
	lGeogrCoord.setY(aCentrCoord.y() + aDecartCoord.x()*180/cos(aCentrCoord.x()*pi/180)/ERadius/pi);
}

float ZInterception::getDistance(QPointF aDecartCoordA, QPointF aDecartCoordB)
{

	float aTempA, aTempB;
	aTempA=aDecartCoordA.x()-aDecartCoordB.x();
	aTempB=aDecartCoordA.y()-aDecartCoordB.y();
	return (qSqrt(aTempA*aTempA+aTempB*aTempB));

}

void ZInterception::getAngle(QPointF aDecartCoordA, QPointF aDecartCoordB, float aSpeed, QPointF &lXYSpeed, float &lAngle)
{
	float pi=3.1415926535897932385F;
	QPointF aTemp;
	aTemp=QPointF(aDecartCoordB.x()-aDecartCoordA.x(), aDecartCoordB.y() - aDecartCoordA.y());
	lAngle=(qAtan2(aTemp.x(),aTemp.y()))/pi*180;
	lXYSpeed=QPointF(qSin(lAngle/180*pi)*aSpeed, qCos(lAngle/180*pi)*aSpeed);
}

void ZInterception::getNextCoord(QPointF aDecartCoordAim, float aAngle, float aSpeed, int aTimeSec, QPointF &lDecartCoord)
{
	float pi=3.1415926535897932385F;

	float aXSpd, aYSpd;
	aXSpd=qSin(aAngle/180*pi)*aSpeed;
	aYSpd=qCos(aAngle/180*pi)*aSpeed;

	float aXPath, aYPath;
	aXPath=aXSpd*aTimeSec;
	aYPath=aYSpd*aTimeSec;

	lDecartCoord=QPointF(aDecartCoordAim.x()+aXPath, aDecartCoordAim.y()+aYPath);
}





