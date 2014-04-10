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
//    qDebug()<<"– ‚Äî– ¬∞– —ó– —ï– ¬ª– –Ö– ¬µ– –Ö–°‚Äπ – “ë– ¬∞– –Ö– –Ö–°‚Äπ– ¬µ – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö–°‚Ä°– —ë– —î– ¬∞";
    mCoordCurrentIntc=aCoordCurrentIntc; //qDebug()<<"– —ô– —ï– —ï–°–Ç– “ë– —ë– –Ö– ¬∞–°‚Äö–°‚Äπ "<<aCoordCurrentIntc.x()<<" "<<aCoordCurrentIntc.y();
    mHgtCurrentIntc=aHgtCurrentIntc; //qDebug()<<"– ‚Äô–°‚Äπ–°–É– —ï–°‚Äö– ¬∞ "<<aHgtCurrentIntc;
    mMaxSpd=aMaxSpd; //qDebug()<<"– –é– —î– —ï–°–Ç– —ï–°–É–°‚Äö–°–ä "<<aMaxSpd;
    mMaxHgtSpd=aMaxHgtSpd; //qDebug()<<"– –é– —î– —ï–°–Ç– —ï–°–É–°‚Äö–°–ä – –Ö– ¬∞– ¬±– —ï–°–Ç– ¬∞ – –Ü–°‚Äπ–°–É– —ï–°‚Äö–°‚Äπ"<<aMaxHgtSpd;

}

void ZInterception::getAimData(QPointF aCoordCurrentAim, float aHgtCurrentAim, float aSpdAim, float aCourseAim)
{
    //qDebug()<<"– ‚Äî– ¬∞– —ó– —ï– ¬ª– –Ö– ¬µ– –Ö–°‚Äπ – “ë– ¬∞– –Ö– –Ö–°‚Äπ– ¬µ – ¬¶– ¬µ– ¬ª– —ë";
    mCoordCurrentAim=aCoordCurrentAim; //qDebug()<<"– —ô– —ï– —ï–°–Ç– “ë– —ë– –Ö– ¬∞–°‚Äö–°‚Äπ "<<aCoordCurrentAim.x()<<" "<<aCoordCurrentAim.y();
    mHgtCurrentAim=aHgtCurrentAim; //qDebug()<<"– ‚Äô–°‚Äπ–°–É– —ï–°‚Äö– ¬∞ "<<aHgtCurrentAim;
    mCurrentAimSpd=aSpdAim; //qDebug()<<"– –é– —î– —ï–°–Ç– —ï–°–É–°‚Äö–°–ä "<<aSpdAim;
    mCurrentAimCourse=aCourseAim; //qDebug()<<"– —ô–°—ì–°–Ç–°–É "<<aCourseAim;

}

void ZInterception::mainProcessing()
{
    //qDebug()<<"– —ü– —ï–°‚Ç¨– ¬ª– ¬∞ – —ï–°–É– –Ö– —ï– –Ü– –Ö– ¬∞–°–è – —ï– ¬±–°–Ç– ¬∞– ¬±– —ï–°‚Äö– —î– ¬∞";

    int aAllTime=60*60;  //– ¬ß– ¬∞–°–É – –Ö– ¬∞ – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö

    QPointF aTempInterceptionCoord;
    QPointF aDecartCoordAim=QPointF(0,0);               //– ‚Äù– ¬µ– —î– ¬∞–°–Ç–°‚Äö– —ï– –Ü–°‚Äπ – —î– —ï– —ï–°–Ç– —ë– “ë– –Ö– ¬∞–°‚Äö–°‚Äπ –°‚Ä – ¬µ– ¬ª– —ë
    QPointF aDecartCoordIntc;                           //– ‚Äù– ¬µ– —î– ¬∞–°–Ç–°‚Äö– —ï– –Ü–°‚Äπ – —î– —ï– —ï–°–Ç– “ë– —ë– –Ö– ¬∞–°‚Äö–°‚Äπ – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö–°‚Ä°– —ë– —î– ¬∞
    geogrToDecart(mCoordCurrentIntc, aDecartCoordIntc, mCoordCurrentAim);

    //–  – ¬∞–°–É–°‚Ä°– ¬µ–°‚Äö – —î– —ï– —ï–°–Ç– “ë– —ë– –Ö– ¬∞–°‚Äö – —î– ¬∞– ¬∂– “ë– —ï– ‚Ññ –°‚Äö– —ï–°‚Ä°– —î– —ë –°‚Ä – ¬µ– ¬ª– —ë – –Ü –°‚Äö– ¬µ–°‚Ä°– ¬µ– –Ö– —ë– ¬µ –°‚Ä°– ¬∞–°–É– ¬∞
    for (uint i=0; i<aAllTime; i++)
    {
		float aTempDistance;                                //–  – ¬∞–°–É–°–É–°‚Äö– —ï–°–è– –Ö– —ë– ¬µ, – —î– —ï–°‚Äö– —ï–°–Ç– —ï– ¬µ – —ó–°–Ç– —ï– ¬ª– ¬µ–°‚Äö– ¬µ– ¬ª– ¬∞ –°‚Ä – ¬µ– ¬ª–°–ä
		float aTempTimeToIntercept;                         //– ‚Äô–°–Ç– ¬µ– —ò–°–è, – –Ö– ¬µ– —ï– ¬±–°‚Ä¶– —ï– “ë– —ë– —ò– —ï– ¬µ – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö–°‚Ä°– —ë– —î–°—ì, –°‚Ä°–°‚Äö– —ï– ¬±–°‚Äπ – “ë– —ï– ¬ª– ¬µ–°‚Äö– ¬µ–°‚Äö–°–ä – “ë– —ï –°‚Äö– —ï–°‚Ä°– —î– —ë – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö– ¬∞
		float aTempTimeToHgtIntercept;                      //– ‚Äô–°–Ç– ¬µ– —ò–°–è, – –Ö– ¬µ– —ï– ¬±–°‚Ä¶– —ï– “ë– —ë– —ò– —ï– ¬µ – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö–°‚Ä°– —ë– —î–°—ì, –°‚Ä°–°‚Äö– —ï– ¬±–°‚Äπ – –Ö– ¬∞– ¬±–°–Ç– ¬∞–°‚Äö–°–ä – –Ü–°‚Äπ–°–É– —ï–°‚Äö–°—ì –°‚Äö– —ï–°‚Ä°– —î– —ë – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö– ¬∞

        //qDebug()<<"I – –Ü – —ï–°–É– –Ö– —ï– –Ü– –Ö– —ï– ‚Ññ – —ï– ¬±–°–Ç– ¬∞– ¬±– —ï–°‚Äö– —î– ¬µ "<<i;

        //– —û– ¬µ– —î–°—ì–°‚Ä∞– ¬∞–°–è – —î– —ï– —ï–°–Ç– “ë– —ë– –Ö– ¬∞–°‚Äö– ¬∞ –°‚Ä – ¬µ– ¬ª– —ë
        getNextCoord(aDecartCoordAim, mCurrentAimCourse, mCurrentAimSpd, i, aTempInterceptionCoord);
        if (mCurrentAimSpd<0.1) aTempInterceptionCoord=aDecartCoordAim;

        //–  – ¬∞–°–É–°–É–°‚Äö– —ï–°–è– –Ö– —ë– ¬µ – —ï–°‚Äö –°‚Äö– ¬µ– —î–°—ì–°‚Ä∞– ¬µ– ‚Ññ – —î– —ï– —ï–°–Ç– “ë– —ë– –Ö– ¬∞–°‚Äö–°‚Äπ –°‚Ä – ¬µ– ¬ª– —ë – “ë– —ï – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö–°‚Ä°– —ë– —î– ¬∞
        aTempDistance=getDistance(aTempInterceptionCoord, aDecartCoordIntc);
        //qDebug()<<" aTempDistance="<<aTempDistance;

        //– —õ– —ó–°–Ç– ¬µ– “ë– ¬µ– ¬ª– ¬µ– –Ö– —ë– ¬µ – –Ü–°–Ç– ¬µ– —ò– ¬µ– –Ö– —ë, – ¬∑– ¬∞ – —î– —ï–°‚Äö– —ï–°–Ç– —ï– ¬µ –°–É–°–ã– “ë– ¬∞ – “ë– —ï– ¬ª– ¬µ–°‚Äö– —ë–°‚Äö – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö–°‚Ä°– —ë– —î
        aTempTimeToIntercept=aTempDistance/mMaxSpd;
        //qDebug()<<" aTempTimeToIntercept="<<aTempTimeToIntercept;

        //– —õ– —ó–°–Ç– ¬µ– “ë– ¬µ– ¬ª– ¬µ– –Ö– —ë– ¬µ – –Ü–°–Ç– ¬µ– —ò– ¬µ– –Ö– —ë, – ¬∑– ¬∞ – —î– —ï–°‚Äö– —ï–°–Ç– —ï– ¬µ – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö–°‚Ä°– —ë– —î – –Ö– ¬∞– ¬±– ¬µ–°–Ç– ¬µ–°‚Äö – –Ü–°‚Äπ–°–É– —ï–°‚Äö–°—ì
        aTempTimeToHgtIntercept=(mHgtCurrentAim+1000-mHgtCurrentIntc)/mMaxHgtSpd;

        //– —õ– —ó–°–Ç– ¬µ– “ë– ¬µ– ¬ª– ¬µ– –Ö– —ë– ¬µ –°—ì–°–É– ¬ª– —ï– –Ü– —ë–°–è – –Ü–°‚Äπ– —ó– —ï– ¬ª– –Ö– ¬µ– –Ö– —ë–°–è – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö– ¬∞
        if (i>aTempTimeToIntercept && i>aTempTimeToHgtIntercept)
        {
            //– —ü– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö – –Ü– —ï– ¬∑– —ò– —ï– ¬∂– ¬µ– –Ö, – —ï– —ó–°–Ç– ¬µ– “ë– ¬µ– ¬ª–°–è– ¬µ– —ò – —î–°—ì–°–Ç–°–É, –°–É– —î– —ï–°–Ç– —ï–°–É–°‚Äö–°–ä, – –Ü–°‚Äπ–°–É– —ï–°‚Äö–°—ì – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö–°‚Ä°– —ë– —î– ¬∞
            QPointF aIntcCoords=aTempInterceptionCoord;               //– —ô– —ï– —ï–°–Ç– “ë– —ë– –Ö– ¬∞–°‚Äö–°‚Äπ – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö– ¬∞
            float aCurrentAngle;                                      //– —ú– ¬µ– —ï– ¬±–°‚Ä¶– —ï– “ë– —ë– —ò–°‚Äπ– ‚Ññ – —î–°—ì–°–Ç–°–É– —ï– –Ü– —ï– ‚Ññ –°—ì– —ñ– —ï– ¬ª – “ë– ¬ª–°–è – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö–°‚Ä°– —ë– —î– ¬∞
            QPointF aXYSpeed;
            getAngle(aDecartCoordIntc, aIntcCoords, mMaxSpd, aXYSpeed, aCurrentAngle);

            float aCurrentHgtInt=i*mMaxHgtSpd;
            float aRadius=aCurrentHgtInt-mHgtCurrentAim;

            //– —õ–°‚Äö– —ó–°–Ç– ¬∞– –Ü– —î– ¬∞ –°–É– —ë– —ñ– –Ö– ¬∞– ¬ª– ¬∞ –°–É – “ë– ¬∞– –Ö– –Ö–°‚Äπ– —ò– —ë – —ï –°‚Äö– —ï–°‚Ä°– —î– ¬µ – —ó– ¬µ–°–Ç– ¬µ–°‚Ä¶– –Ü– ¬∞–°‚Äö– ¬∞
            decartToGeogr(aTempInterceptionCoord, aIntcCoords, mCoordCurrentAim);

//            emit signal_sendInterceptionPoint(aIntcCoords, aCurrentHgtInt, aRadius, i, aCurrentAngle, mMaxSpd);
            _client->perehvat_point(_bla, _bpla, aIntcCoords, aCurrentHgtInt, aRadius, i, aCurrentAngle, mMaxSpd);
            //– —ü–°–Ç– —ë–°–É– –Ü– —ï– ¬µ– –Ö– —ë– ¬µ – –Ü–°–Ç– ¬µ– —ò– ¬µ– –Ö– —ë – “ë– ¬ª–°–è – –Ü–°‚Äπ–°‚Ä¶– —ï– “ë– ¬∞ – —ë– ¬∑ –°‚Ä – —ë– —î– ¬ª– ¬∞
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





