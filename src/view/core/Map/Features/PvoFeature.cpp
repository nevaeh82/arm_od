#include <Logger/Logger.h>

#define _USE_MATH_DEFINES
#include "math.h"

#include "Map/IMapStyleManager.h"
#include "Map/Features/PvoFeature.h"

namespace MapFeature {

PvoFeature::PvoFeature(IObjectsFactory* factory, PwGisWidget* pw, const NStation &niipp, PwGisStyle *styleSimplePvo, PwGisStyle *styleActivePvo, PwGisStyle *styleSimpleReb, PwGisStyle *styleActiveReb)
    : FeatureAbstract( factory, QString("nStation%1").arg(niipp.getID()),
                       QString("nStation%1").arg(niipp.getID()) )
    , m_niippId( niipp.getID() )
    , m_mode( (Mode) niipp.getMode() )
    , m_params(niipp)
    , m_pwgis(pw)
    , m_styleSimplePvo(styleSimplePvo)
    , m_styleActivePvo(styleActivePvo)
    , m_styleSimpleReb(styleSimpleReb)
    , m_styleActiveReb(styleActiveReb)
{
	m_circle = factory->createCircle();
    m_sector = factory->createSector();
    m_marker = factory->createMarker();

    m_blinkTimer = new QTimer(this);
    m_blinkTimer->setInterval(10000);

    m_styleBlinkTimer = new QTimer(this);
    m_styleBlinkTimer->setInterval(100);


    m_isBlink = false;
    m_isIntersect = false;

    if(m_mode == Mode::Pvo) {
        m_sector->addStyleByName( QString(MAP_STYLE_NAME_PVO).arg(0)  );
        m_marker->addStyleByName( QString(MAP_STYLE_NAME_PVO).arg(0)  );
        m_name = QString("PVO%1").arg(m_params.getID());
    } else {
        m_sector->addStyleByName( QString(MAP_STYLE_NAME_REB).arg(0) );
        m_marker->addStyleByName( QString(MAP_STYLE_NAME_REB).arg(0)  );
        m_name = QString("REB%1").arg(m_params.getID());
    }

    if(m_mode == Mode::Pvo) {
        m_circle->addStyleByName( QString(MAP_STYLE_NAME_PVO).arg(0) );
    } else {
        m_circle->addStyleByName( QString(MAP_STYLE_NAME_REB).arg(0)  );
    }

    connect(m_blinkTimer, SIGNAL(timeout()), this, SLOT(onBlinkTimeout()));
    connect(m_styleBlinkTimer, SIGNAL(timeout()), this, SLOT(onBlinking()));

    updateMap();
}

PvoFeature::~PvoFeature()
{
	m_circle->removeFromMap();
	delete m_circle;

	m_sector->removeFromMap();
	delete m_sector;

    m_marker->removeFromMap();
    delete m_marker;
}

void PvoFeature::setPosition(const QPointF& position)
{
    FeatureAbstract::setPosition( QPointF( position.y(), position.x() ) );

    m_circle->setOriginPoint( &m_position );
    m_sector->setOriginPoint( &m_position );
    m_marker->setPosition( &m_position );
}

void PvoFeature::setRadius(double value)
{
    m_radius = value;
    m_circle->setRadius( m_radius );
    m_sector->setRadius( m_radius );
}

void PvoFeature::update(const NStation& niipp)
{
    m_params = niipp;
    updateMap();
}

void PvoFeature::setEnemyPoint(QString id, const UavInfo& uav)
{
    m_enemyPointList.insert(id, uav);

    checkIntersectons();
}

void PvoFeature::checkIntersectons()
{
    foreach (QString enemyId, m_enemyPointList.keys()) {
        UavInfo ptf = m_enemyPointList.value(enemyId);
        if(m_myPolygon.containsPoint(QPointF(ptf.lon, ptf.lat), Qt::OddEvenFill)) {
            //m_params.setName(m_params.getName() + "\n" + QString("Found enemy %1").arg(enemyId));

            m_isIntersect = true;
            m_blinkTimer->start();
            m_styleBlinkTimer->start();

            double azimuth = getAzimuthOnSphere(m_params.getPos().y(), m_params.getPos().x(),
                               ptf.lon, ptf.lat);
            double distance = getDistanceOnSphere(m_params.getPos().y(), m_params.getPos().x(),
                               ptf.lon, ptf.lat);

           //Send Niipp with upHeader
            if(m_tcpClientManager) {
                m_tcpClientManager->sendEnemyIntersectToServer(ptf, azimuth, distance, m_params.getName());
                m_tcpClientManager->sendEnemyIntersectToClient(ptf, azimuth, distance, m_params.getName());
            }

        } else {
            //send Niipp only
//             if(m_tcpClientManager) {
//                m_tcpClientManager->sendEnemyToServer(ptf);
//                m_tcpClientManager->sendEnemyToClient(ptf);
//             }
        }
    }
}

void PvoFeature::onBlinking()
{
    m_sector->removeStyle(m_styleSimpleReb);
    m_sector->removeStyle(m_styleActiveReb);
    m_sector->removeStyle(m_styleActivePvo);
    m_sector->removeStyle(m_styleSimplePvo);
    m_circle->removeStyle(m_styleSimpleReb);
    m_circle->removeStyle(m_styleActiveReb);
    m_circle->removeStyle(m_styleActivePvo);
    m_circle->removeStyle(m_styleSimplePvo);


    if(m_params.getMode() == Mode::Pvo) {
       // m_sector->addStyleByName( QString(MAP_STYLE_NAME_PVO).arg(0)  );
        if(m_isBlink) {
            m_sector->addStyle(m_styleSimplePvo);
        } else {
            m_sector->addStyle(m_styleActivePvo);
        }
    } else {
       // m_sector->addStyleByName( QString(MAP_STYLE_NAME_REB).arg(0) );
        if(m_isBlink) {
            m_sector->addStyle(m_styleSimpleReb);
        } else {
            m_sector->addStyle(m_styleActiveReb);
        }
    }

    if(m_params.getMode() == Mode::Pvo) {
        //m_circle->addStyleByName( QString(MAP_STYLE_NAME_PVO).arg(0) );
        if(m_isBlink) {
            m_circle->addStyle(m_styleSimplePvo);
        } else {
            m_circle->addStyle(m_styleActivePvo);
        }
    } else {
        //m_circle->addStyleByName( QString(MAP_STYLE_NAME_REB).arg(0)  );
        if(m_isBlink) {
            m_circle->addStyle(m_styleSimpleReb);
        } else {
            m_circle->addStyle(m_styleActiveReb);
        }
    }

    m_sector->removeFromMap();
    m_sector->updateMap();

    if(m_params.getIsCircle()) {
        m_circle->removeFromMap();
        m_circle->updateMap();
    }

    m_isBlink = !m_isBlink;
}


void PvoFeature::onBlinkTimeout()
{
    m_isIntersect = false;

    m_blinkTimer->stop();
    m_styleBlinkTimer->stop();

    updateMap();
}

void PvoFeature::setAzimuth(int azimuth, int width)
{
        double startAngle = azimuth + width/2;

        if ( startAngle < 0 ) {
            startAngle = 360 + startAngle;
        }

        startAngle *= -1;
        startAngle += 90;

        double endAngle = startAngle + width;

        m_sector->setStartAngle( startAngle );
        m_sector->setEndAngle( endAngle );
}

void PvoFeature::updateMap()
{
    setAzimuth(m_params.getAzimuth(), m_params.getWidth());

    setPosition(m_params.getPos());

    setRadius(m_params.getRadius());
    m_marker->setName(m_params.getName());



    m_myPolygon.clear();
    if(m_params.getIsCircle() ) {

        for(int i = 0; i<359; i++) {
            m_myPolygon.append(getPointByDistance(m_params.getPos().y(), m_params.getPos().x(),
                                                 m_params.getRadius(), i));
        }


    } else {
        m_myPolygon.append(QPointF(m_params.getPos().y(), m_params.getPos().x()));
        int angleStart = m_params.getAzimuth() - m_params.getWidth()/2;
        int angleEnd = m_params.getAzimuth() + m_params.getWidth()/2;

//        if(angleStart < 0 ) {
//            angleStart = 360 + angleStart;
//        }

        for(int i = angleStart; i<angleEnd; i++) {
            m_myPolygon.append(getPointByDistance(m_params.getPos().y(), m_params.getPos().x(),
                                                 m_params.getRadius(), i));
        }

//        m_myPolygon.append(getPointByDistance(m_params.getPos().y(), m_params.getPos().x(),
//                                                 m_params.getRadius(), angleStart));
//        m_myPolygon.append(getPointByDistance(m_params.getPos().y(), m_params.getPos().x(),
//                                                 m_params.getRadius(), angleEnd));
    }



        m_circle->removeFromMap();
        m_sector->removeFromMap();
        m_marker->removeFromMap();

    m_sector->removeStyle(m_styleSimpleReb);
    m_sector->removeStyle(m_styleActiveReb);
    m_sector->removeStyle(m_styleSimplePvo);
    m_sector->removeStyle(m_styleActivePvo);
    //==========================================
    m_circle->removeStyle(m_styleSimpleReb);
    m_circle->removeStyle(m_styleActiveReb);
    m_circle->removeStyle(m_styleSimplePvo);
    m_circle->removeStyle(m_styleActivePvo);
    //==========================================
    m_marker->removeStyle(m_styleSimplePvo);
    m_marker->removeStyle(m_styleSimpleReb);


    if(m_params.getMode() == Mode::Pvo) {
       // m_sector->addStyleByName( QString(MAP_STYLE_NAME_PVO).arg(0)  );
        m_sector->addStyle(m_styleSimplePvo);
        m_marker->addStyle(m_styleSimplePvo);
    } else {
       // m_sector->addStyleByName( QString(MAP_STYLE_NAME_REB).arg(0) );
        m_sector->addStyle(m_styleSimpleReb);
        m_marker->addStyle(m_styleSimpleReb);
    }

    if(m_params.getMode() == Mode::Pvo) {
        //m_circle->addStyleByName( QString(MAP_STYLE_NAME_PVO).arg(0) );
        m_circle->addStyle(m_styleSimplePvo);
    } else {
        //m_circle->addStyleByName( QString(MAP_STYLE_NAME_REB).arg(0)  );
        m_circle->addStyle(m_styleSimpleReb);
    }



    if(m_params.getIsEnable()) {
        m_sector->updateMap();
        m_marker->updateMap();

        if(m_params.getIsCircle()) {
            m_circle->updateMap();
        } else {
            m_circle->removeFromMap();
        }
    }
}

void PvoFeature::removeFromMap()
{
	m_circle->removeFromMap();
	m_sector->removeFromMap();
}


double PvoFeature::getDistanceOnSphere(double lon1, double lat1,
                                     double lon2, double lat2)
{
    double retVal = -1;

    double lonRad1 = lon1*M_PI/180.0;
    double latRad1 = lat1*M_PI/180.0;
    double lonRad2 = lon2*M_PI/180.0;
    double latRad2 = lat2*M_PI/180.0;

    double cosLat1 = cos(latRad1);
    double cosLat2 = cos(latRad2);
    double sinLat1 = sin(latRad1);
    double sinLat2 = sin(latRad2);
    double deltaLon = lonRad2 - lonRad1;
    double cosDelta = cos(deltaLon);
    double sinDelta = sin(deltaLon);

    //calculate great-circles
    double y = sqrt(pow(cosLat2*sinDelta, 2.0) + pow(cosLat1*sinLat2 - sinLat1*cosLat2*cosDelta, 2.0));
    double x = sinLat1*sinLat2 + cosLat1*cosLat2*cosDelta;
    double earthArc = atan2(y,x);

    double r = 6378137.0;

    retVal = earthArc*r;

    return retVal;
}



double PvoFeature::getAzimuthOnSphere(const double lon1, const double lat1,
                                    const double lon2, const double lat2)
{
    //-----перехожу в локальную систему координат с центром в lon1,lat1
    double originLon = lon1;
    double originLat = lat1;
    double r = 6378137.0;
    //----------------
    double one_degree_lat = r*M_PI/180.0;
    double one_degree_lon = one_degree_lat*cos(originLat*M_PI/180.0);

    //----------------
    double x1 = 0.0;
    double y1 = 0.0;

    //----------------
    double x2 = (lon2 - originLon)*one_degree_lon;
    double y2 = (lat2 - originLat)*one_degree_lat;

    //----------------
    return RadianToDegree(getAzimuth(x1, y1, x2, y2));
}

bool doubleEqual(double dFirstVal, double dSecondVal)
{
    return std::fabs(dFirstVal - dSecondVal) < std::numeric_limits<double>::epsilon();
}

double PvoFeature::getAzimuth(const double x1, const double y1,
                            const double x2, const double y2)
{
    double x, y, tan_alpha, alpha;

    if (doubleEqual(x1, x2) && doubleEqual(y1, y2)) {
        return 0.0;
    }

    if (doubleEqual(x1, x2) && (y1 < y2)) {
        return 0.0;
    }

    if ((x1 < x2) && (y1 < y2)) {
        y = y2 - y1;
        x = x2 - x1;
        tan_alpha = x/y;
        alpha = atan(tan_alpha);
        return alpha;
    }

    if ((x1 < x2) && doubleEqual(y1, y2)) {
        return (M_PI/2.0);
    }

    if ((x1 < x2) && (y1 > y2)) {
        y = y1 - y2;
        x = x2 - x1;
        tan_alpha = y/x;
        alpha = atan(tan_alpha) + M_PI/2.0;
        return alpha;
    }

    if (doubleEqual(x1, x2) && (y1 > y2)) {
        return M_PI;
    }

    if ((x1 > x2) && (y1 > y2)) {
        y = y1 - y2;
        x = x1 - x2;
        tan_alpha = x/y;
        alpha = atan(tan_alpha) + M_PI;
        return alpha;
    }

    if ((x1 > x2) && doubleEqual(y1, y2)) {
        return (1.5*M_PI);
    }

    if ((x1 > x2) && (y1 < y2)) {
        y = y2 - y1;
        x = x1 - x2;
        tan_alpha = y/x;
        alpha = atan(tan_alpha) + 1.5*M_PI;
        return alpha;
    }

    return 0.0;
}



QPointF PvoFeature::getPointByDistance(double lon1, double lat1, double dist, double azimuth)
{
    dist *= 1;
    lon1 = DegreeToRadian(lon1);
    lat1 = DegreeToRadian(lat1);
    azimuth = DegreeToRadian( getCorrectDegAngle(azimuth) );
    double angle = DegreeToRadian(90);

    double b = dist / 6378137.0; //meter;
    double a = acos( cos(b)*cos(angle-lat1) + sin(angle-lat1)*sin(b)*cos(azimuth) );
    double B = asin( sin(b)*sin(azimuth)/sin(a) );

    return QPointF( RadianToDegree(B+lon1), RadianToDegree(angle-a) );
}

void PvoFeature::setTcpClientManager(TcpClientManager *manager)
{
    m_tcpClientManager = manager;
}

double PvoFeature::DegreeToRadian(double angle)
{
    return M_PI * angle / 180.0;
}

double PvoFeature::RadianToDegree(double angle)
{
    return 180.0 * angle / M_PI;
}

double PvoFeature::getCorrectDegAngle(const double deg_1)
{
    double retVal = deg_1;
    if (deg_1 < 0.0) {
        retVal = deg_1 + 360.0;
    }
    else if (deg_1 > 360.0) {
        retVal = deg_1 - 360.0;
    }
    return retVal;
}


} // namespace MapFeature


