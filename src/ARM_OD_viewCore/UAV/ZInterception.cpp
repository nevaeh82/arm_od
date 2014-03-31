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
//    qDebug()<<"Р—Р°РїРѕР»РЅРµРЅС‹ РґР°РЅРЅС‹Рµ РїРµСЂРµС…РІР°С‚С‡РёРєР°";
    mCoordCurrentIntc=aCoordCurrentIntc; //qDebug()<<"РљРѕРѕСЂРґРёРЅР°С‚С‹ "<<aCoordCurrentIntc.x()<<" "<<aCoordCurrentIntc.y();
    mHgtCurrentIntc=aHgtCurrentIntc; //qDebug()<<"Р’С‹СЃРѕС‚Р° "<<aHgtCurrentIntc;
    mMaxSpd=aMaxSpd; //qDebug()<<"РЎРєРѕСЂРѕСЃС‚СЊ "<<aMaxSpd;
    mMaxHgtSpd=aMaxHgtSpd; //qDebug()<<"РЎРєРѕСЂРѕСЃС‚СЊ РЅР°Р±РѕСЂР° РІС‹СЃРѕС‚С‹"<<aMaxHgtSpd;

}

void ZInterception::getAimData(QPointF aCoordCurrentAim, float aHgtCurrentAim, float aSpdAim, float aCourseAim)
{
    //qDebug()<<"Р—Р°РїРѕР»РЅРµРЅС‹ РґР°РЅРЅС‹Рµ Р¦РµР»Рё";
    mCoordCurrentAim=aCoordCurrentAim; //qDebug()<<"РљРѕРѕСЂРґРёРЅР°С‚С‹ "<<aCoordCurrentAim.x()<<" "<<aCoordCurrentAim.y();
    mHgtCurrentAim=aHgtCurrentAim; //qDebug()<<"Р’С‹СЃРѕС‚Р° "<<aHgtCurrentAim;
    mCurrentAimSpd=aSpdAim; //qDebug()<<"РЎРєРѕСЂРѕСЃС‚СЊ "<<aSpdAim;
    mCurrentAimCourse=aCourseAim; //qDebug()<<"РљСѓСЂСЃ "<<aCourseAim;

}

void ZInterception::mainProcessing()
{
    //qDebug()<<"РџРѕС€Р»Р° РѕСЃРЅРѕРІРЅР°СЏ РѕР±СЂР°Р±РѕС‚РєР°";

    int aAllTime=60*60;  //Р§Р°СЃ РЅР° РїРµСЂРµС…РІР°С‚

    QPointF aTempInterceptionCoord;
    QPointF aDecartCoordAim=QPointF(0,0);               //Р”РµРєР°СЂС‚РѕРІС‹ РєРѕРѕСЂРёРґРЅР°С‚С‹ С†РµР»Рё
    QPointF aDecartCoordIntc;                           //Р”РµРєР°СЂС‚РѕРІС‹ РєРѕРѕСЂРґРёРЅР°С‚С‹ РїРµСЂРµС…РІР°С‚С‡РёРєР°
    geogrToDecart(mCoordCurrentIntc, aDecartCoordIntc, mCoordCurrentAim);

    //Р Р°СЃС‡РµС‚ РєРѕРѕСЂРґРёРЅР°С‚ РєР°Р¶РґРѕР№ С‚РѕС‡РєРё С†РµР»Рё РІ С‚РµС‡РµРЅРёРµ С‡Р°СЃР°
    for (uint i=0; i<aAllTime; i++)
    {
		float aTempDistance;                                //Р Р°СЃСЃС‚РѕСЏРЅРёРµ, РєРѕС‚РѕСЂРѕРµ РїСЂРѕР»РµС‚РµР»Р° С†РµР»СЊ
		float aTempTimeToIntercept;                         //Р’СЂРµРјСЏ, РЅРµРѕР±С…РѕРґРёРјРѕРµ РїРµСЂРµС…РІР°С‚С‡РёРєСѓ, С‡С‚РѕР±С‹ РґРѕР»РµС‚РµС‚СЊ РґРѕ С‚РѕС‡РєРё РїРµСЂРµС…РІР°С‚Р°
		float aTempTimeToHgtIntercept;                      //Р’СЂРµРјСЏ, РЅРµРѕР±С…РѕРґРёРјРѕРµ РїРµСЂРµС…РІР°С‚С‡РёРєСѓ, С‡С‚РѕР±С‹ РЅР°Р±СЂР°С‚СЊ РІС‹СЃРѕС‚Сѓ С‚РѕС‡РєРё РїРµСЂРµС…РІР°С‚Р°

        //qDebug()<<"I РІ РѕСЃРЅРѕРІРЅРѕР№ РѕР±СЂР°Р±РѕС‚РєРµ "<<i;

        //РўРµРєСѓС‰Р°СЏ РєРѕРѕСЂРґРёРЅР°С‚Р° С†РµР»Рё
        getNextCoord(aDecartCoordAim, mCurrentAimCourse, mCurrentAimSpd, i, aTempInterceptionCoord);
        if (mCurrentAimSpd<0.1) aTempInterceptionCoord=aDecartCoordAim;

        //Р Р°СЃСЃС‚РѕСЏРЅРёРµ РѕС‚ С‚РµРєСѓС‰РµР№ РєРѕРѕСЂРґРёРЅР°С‚С‹ С†РµР»Рё РґРѕ РїРµСЂРµС…РІР°С‚С‡РёРєР°
        aTempDistance=getDistance(aTempInterceptionCoord, aDecartCoordIntc);
        //qDebug()<<" aTempDistance="<<aTempDistance;

        //РћРїСЂРµРґРµР»РµРЅРёРµ РІСЂРµРјРµРЅРё, Р·Р° РєРѕС‚РѕСЂРѕРµ СЃСЋРґР° РґРѕР»РµС‚РёС‚ РїРµСЂРµС…РІР°С‚С‡РёРє
        aTempTimeToIntercept=aTempDistance/mMaxSpd;
        //qDebug()<<" aTempTimeToIntercept="<<aTempTimeToIntercept;

        //РћРїСЂРµРґРµР»РµРЅРёРµ РІСЂРµРјРµРЅРё, Р·Р° РєРѕС‚РѕСЂРѕРµ РїРµСЂРµС…РІР°С‚С‡РёРє РЅР°Р±РµСЂРµС‚ РІС‹СЃРѕС‚Сѓ
        aTempTimeToHgtIntercept=(mHgtCurrentAim+1000-mHgtCurrentIntc)/mMaxHgtSpd;

        //РћРїСЂРµРґРµР»РµРЅРёРµ СѓСЃР»РѕРІРёСЏ РІС‹РїРѕР»РЅРµРЅРёСЏ РїРµСЂРµС…РІР°С‚Р°
        if (i>aTempTimeToIntercept && i>aTempTimeToHgtIntercept)
        {
            //РџРµСЂРµС…РІР°С‚ РІРѕР·РјРѕР¶РµРЅ, РѕРїСЂРµРґРµР»СЏРµРј РєСѓСЂСЃ, СЃРєРѕСЂРѕСЃС‚СЊ, РІС‹СЃРѕС‚Сѓ РїРµСЂРµС…РІР°С‚С‡РёРєР°
            QPointF aIntcCoords=aTempInterceptionCoord;               //РљРѕРѕСЂРґРёРЅР°С‚С‹ РїРµСЂРµС…РІР°С‚Р°
            float aCurrentAngle;                                      //РќРµРѕР±С…РѕРґРёРјС‹Р№ РєСѓСЂСЃРѕРІРѕР№ СѓРіРѕР» РґР»СЏ РїРµСЂРµС…РІР°С‚С‡РёРєР°
            QPointF aXYSpeed;
            getAngle(aDecartCoordIntc, aIntcCoords, mMaxSpd, aXYSpeed, aCurrentAngle);

            float aCurrentHgtInt=i*mMaxHgtSpd;
            float aRadius=aCurrentHgtInt-mHgtCurrentAim;

            //РћС‚РїСЂР°РІРєР° СЃРёРіРЅР°Р»Р° СЃ РґР°РЅРЅС‹РјРё Рѕ С‚РѕС‡РєРµ РїРµСЂРµС…РІР°С‚Р°
            decartToGeogr(aTempInterceptionCoord, aIntcCoords, mCoordCurrentAim);

//            emit signal_sendInterceptionPoint(aIntcCoords, aCurrentHgtInt, aRadius, i, aCurrentAngle, mMaxSpd);
            _client->perehvat_point(_bla, _bpla, aIntcCoords, aCurrentHgtInt, aRadius, i, aCurrentAngle, mMaxSpd);
            //РџСЂРёСЃРІРѕРµРЅРёРµ РІСЂРµРјРµРЅРё РґР»СЏ РІС‹С…РѕРґР° РёР· С†РёРєР»Р°
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





