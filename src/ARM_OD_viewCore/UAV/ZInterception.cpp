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
	getAimData(track.last(), alt, speed, bearing);

    mainProcessing();
}

void ZInterception::getIntcData(QPointF aCoordCurrentIntc, float aHgtCurrentIntc, float aMaxSpd, float aMaxHgtSpd)
{
//    qDebug()<<"� —� °� ї� ѕ� »� Ѕ� µ� ЅС‹ � ґ� °� Ѕ� ЅС‹� µ � ї� µСЂ� µС…� І� °С‚С‡� ё� є� °";
    mCoordCurrentIntc=aCoordCurrentIntc; //qDebug()<<"� љ� ѕ� ѕСЂ� ґ� ё� Ѕ� °С‚С‹ "<<aCoordCurrentIntc.x()<<" "<<aCoordCurrentIntc.y();
    mHgtCurrentIntc=aHgtCurrentIntc; //qDebug()<<"� ’С‹СЃ� ѕС‚� ° "<<aHgtCurrentIntc;
    mMaxSpd=aMaxSpd; //qDebug()<<"� Ў� є� ѕСЂ� ѕСЃС‚СЊ "<<aMaxSpd;
    mMaxHgtSpd=aMaxHgtSpd; //qDebug()<<"� Ў� є� ѕСЂ� ѕСЃС‚СЊ � Ѕ� °� ±� ѕСЂ� ° � ІС‹СЃ� ѕС‚С‹"<<aMaxHgtSpd;

}

void ZInterception::getAimData(QPointF aCoordCurrentAim, float aHgtCurrentAim, float aSpdAim, float aCourseAim)
{
    //qDebug()<<"� —� °� ї� ѕ� »� Ѕ� µ� ЅС‹ � ґ� °� Ѕ� ЅС‹� µ � ¦� µ� »� ё";
    mCoordCurrentAim=aCoordCurrentAim; //qDebug()<<"� љ� ѕ� ѕСЂ� ґ� ё� Ѕ� °С‚С‹ "<<aCoordCurrentAim.x()<<" "<<aCoordCurrentAim.y();
    mHgtCurrentAim=aHgtCurrentAim; //qDebug()<<"� ’С‹СЃ� ѕС‚� ° "<<aHgtCurrentAim;
    mCurrentAimSpd=aSpdAim; //qDebug()<<"� Ў� є� ѕСЂ� ѕСЃС‚СЊ "<<aSpdAim;
    mCurrentAimCourse=aCourseAim; //qDebug()<<"� љСѓСЂСЃ "<<aCourseAim;

}

void ZInterception::mainProcessing()
{
    //qDebug()<<"� џ� ѕС€� »� ° � ѕСЃ� Ѕ� ѕ� І� Ѕ� °СЏ � ѕ� ±СЂ� °� ±� ѕС‚� є� °";

    int aAllTime=60*60;  //� §� °СЃ � Ѕ� ° � ї� µСЂ� µС…� І� °С‚

    QPointF aTempInterceptionCoord;
    QPointF aDecartCoordAim=QPointF(0,0);               //� ”� µ� є� °СЂС‚� ѕ� ІС‹ � є� ѕ� ѕСЂ� ё� ґ� Ѕ� °С‚С‹ С� � µ� »� ё
    QPointF aDecartCoordIntc;                           //� ”� µ� є� °СЂС‚� ѕ� ІС‹ � є� ѕ� ѕСЂ� ґ� ё� Ѕ� °С‚С‹ � ї� µСЂ� µС…� І� °С‚С‡� ё� є� °
    geogrToDecart(mCoordCurrentIntc, aDecartCoordIntc, mCoordCurrentAim);

    //�  � °СЃС‡� µС‚ � є� ѕ� ѕСЂ� ґ� ё� Ѕ� °С‚ � є� °� ¶� ґ� ѕ� № С‚� ѕС‡� є� ё С� � µ� »� ё � І С‚� µС‡� µ� Ѕ� ё� µ С‡� °СЃ� °
    for (uint i=0; i<aAllTime; i++)
    {
		float aTempDistance;                                //�  � °СЃСЃС‚� ѕСЏ� Ѕ� ё� µ, � є� ѕС‚� ѕСЂ� ѕ� µ � їСЂ� ѕ� »� µС‚� µ� »� ° С� � µ� »СЊ
		float aTempTimeToIntercept;                         //� ’СЂ� µ� јСЏ, � Ѕ� µ� ѕ� ±С…� ѕ� ґ� ё� ј� ѕ� µ � ї� µСЂ� µС…� І� °С‚С‡� ё� єСѓ, С‡С‚� ѕ� ±С‹ � ґ� ѕ� »� µС‚� µС‚СЊ � ґ� ѕ С‚� ѕС‡� є� ё � ї� µСЂ� µС…� І� °С‚� °
		float aTempTimeToHgtIntercept;                      //� ’СЂ� µ� јСЏ, � Ѕ� µ� ѕ� ±С…� ѕ� ґ� ё� ј� ѕ� µ � ї� µСЂ� µС…� І� °С‚С‡� ё� єСѓ, С‡С‚� ѕ� ±С‹ � Ѕ� °� ±СЂ� °С‚СЊ � ІС‹СЃ� ѕС‚Сѓ С‚� ѕС‡� є� ё � ї� µСЂ� µС…� І� °С‚� °

        //qDebug()<<"I � І � ѕСЃ� Ѕ� ѕ� І� Ѕ� ѕ� № � ѕ� ±СЂ� °� ±� ѕС‚� є� µ "<<i;

        //� ў� µ� єСѓС‰� °СЏ � є� ѕ� ѕСЂ� ґ� ё� Ѕ� °С‚� ° С� � µ� »� ё
        getNextCoord(aDecartCoordAim, mCurrentAimCourse, mCurrentAimSpd, i, aTempInterceptionCoord);
        if (mCurrentAimSpd<0.1) aTempInterceptionCoord=aDecartCoordAim;

        //�  � °СЃСЃС‚� ѕСЏ� Ѕ� ё� µ � ѕС‚ С‚� µ� єСѓС‰� µ� № � є� ѕ� ѕСЂ� ґ� ё� Ѕ� °С‚С‹ С� � µ� »� ё � ґ� ѕ � ї� µСЂ� µС…� І� °С‚С‡� ё� є� °
        aTempDistance=getDistance(aTempInterceptionCoord, aDecartCoordIntc);
        //qDebug()<<" aTempDistance="<<aTempDistance;

        //� ћ� їСЂ� µ� ґ� µ� »� µ� Ѕ� ё� µ � ІСЂ� µ� ј� µ� Ѕ� ё, � ·� ° � є� ѕС‚� ѕСЂ� ѕ� µ СЃСЋ� ґ� ° � ґ� ѕ� »� µС‚� ёС‚ � ї� µСЂ� µС…� І� °С‚С‡� ё� є
        aTempTimeToIntercept=aTempDistance/mMaxSpd;
        //qDebug()<<" aTempTimeToIntercept="<<aTempTimeToIntercept;

        //� ћ� їСЂ� µ� ґ� µ� »� µ� Ѕ� ё� µ � ІСЂ� µ� ј� µ� Ѕ� ё, � ·� ° � є� ѕС‚� ѕСЂ� ѕ� µ � ї� µСЂ� µС…� І� °С‚С‡� ё� є � Ѕ� °� ±� µСЂ� µС‚ � ІС‹СЃ� ѕС‚Сѓ
        aTempTimeToHgtIntercept=(mHgtCurrentAim+1000-mHgtCurrentIntc)/mMaxHgtSpd;

        //� ћ� їСЂ� µ� ґ� µ� »� µ� Ѕ� ё� µ СѓСЃ� »� ѕ� І� ёСЏ � ІС‹� ї� ѕ� »� Ѕ� µ� Ѕ� ёСЏ � ї� µСЂ� µС…� І� °С‚� °
        if (i>aTempTimeToIntercept && i>aTempTimeToHgtIntercept)
        {
            //� џ� µСЂ� µС…� І� °С‚ � І� ѕ� ·� ј� ѕ� ¶� µ� Ѕ, � ѕ� їСЂ� µ� ґ� µ� »СЏ� µ� ј � єСѓСЂСЃ, СЃ� є� ѕСЂ� ѕСЃС‚СЊ, � ІС‹СЃ� ѕС‚Сѓ � ї� µСЂ� µС…� І� °С‚С‡� ё� є� °
            QPointF aIntcCoords=aTempInterceptionCoord;               //� љ� ѕ� ѕСЂ� ґ� ё� Ѕ� °С‚С‹ � ї� µСЂ� µС…� І� °С‚� °
            float aCurrentAngle;                                      //� ќ� µ� ѕ� ±С…� ѕ� ґ� ё� јС‹� № � єСѓСЂСЃ� ѕ� І� ѕ� № Сѓ� і� ѕ� » � ґ� »СЏ � ї� µСЂ� µС…� І� °С‚С‡� ё� є� °
            QPointF aXYSpeed;
            getAngle(aDecartCoordIntc, aIntcCoords, mMaxSpd, aXYSpeed, aCurrentAngle);

            float aCurrentHgtInt=i*mMaxHgtSpd;
            float aRadius=aCurrentHgtInt-mHgtCurrentAim;

            //� ћС‚� їСЂ� °� І� є� ° СЃ� ё� і� Ѕ� °� »� ° СЃ � ґ� °� Ѕ� ЅС‹� ј� ё � ѕ С‚� ѕС‡� є� µ � ї� µСЂ� µС…� І� °С‚� °
            decartToGeogr(aTempInterceptionCoord, aIntcCoords, mCoordCurrentAim);

//            emit signal_sendInterceptionPoint(aIntcCoords, aCurrentHgtInt, aRadius, i, aCurrentAngle, mMaxSpd);
            _client->perehvat_point(_bla, _bpla, aIntcCoords, aCurrentHgtInt, aRadius, i, aCurrentAngle, mMaxSpd);
            //� џСЂ� ёСЃ� І� ѕ� µ� Ѕ� ё� µ � ІСЂ� µ� ј� µ� Ѕ� ё � ґ� »СЏ � ІС‹С…� ѕ� ґ� ° � ё� · С� � ё� є� »� °
            i=aAllTime+10;
        }
    }
}


void ZInterception::geogrToDecart(QPointF aGeogrCoord,QPointF &lDecartCoord, QPointF aCentrCoord)
{
    float dY, dX, popravka;
    float pi=3.1415926535897932385;
    float ERadius=6366197.724;

    popravka=cos(aCentrCoord.x()*pi/180);

    dY=(aGeogrCoord.x()-aCentrCoord.x())*pi*ERadius/180;
    dX=(aGeogrCoord.y()-aCentrCoord.y())*pi*ERadius/180;
    lDecartCoord=QPointF(dX*popravka, dY);
}

void ZInterception::decartToGeogr(QPointF aDecartCoord, QPointF &lGeogrCoord, QPointF aCentrCoord)
{
    float pi=3.1415926535897932385;
    float ERadius=6366197.724;

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
    float pi=3.1415926535897932385;
    QPointF aTemp;
    aTemp=QPointF(aDecartCoordB.x()-aDecartCoordA.x(), aDecartCoordB.y() - aDecartCoordA.y());
    lAngle=(qAtan2(aTemp.x(),aTemp.y()))/pi*180;
    lXYSpeed=QPointF(qSin(lAngle/180*pi)*aSpeed, qCos(lAngle/180*pi)*aSpeed);
}

void ZInterception::getNextCoord(QPointF aDecartCoordAim, float aAngle, float aSpeed, int aTimeSec, QPointF &lDecartCoord)
{
    float pi=3.1415926535897932385;

    float aXSpd, aYSpd;
    aXSpd=qSin(aAngle/180*pi)*aSpeed;
    aYSpd=qCos(aAngle/180*pi)*aSpeed;

    float aXPath, aYPath;
    aXPath=aXSpd*aTimeSec;
    aYPath=aYSpd*aTimeSec;

    lDecartCoord=QPointF(aDecartCoordAim.x()+aXPath, aDecartCoordAim.y()+aYPath);
}





