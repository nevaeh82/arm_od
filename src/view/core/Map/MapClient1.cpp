#include <QVariant>

#include <Logger/Logger.h>
#include <math.h>
#include <QMessageBox>

#include "Uav/UavModel.h"
#include "Map/MapStyleManager.h"
#include "Map/MapClient1.h"

#include "PwGis/MapProvider.h"
#include "PwGis/LayerManager.h"


#include "SolverPacket1.pb.h"

#define clearObjectsList(type, map) foreach( type* item, map ) { delete item; } map.clear();

#define HYPERBOLE_LIFE_TIME 5000
#define HYPERBOLEZONE_LIFE_TIME 1000

MapClient1::MapClient1(MapWidget* pwWidget, Station* station, QObject* parent)
	: QObject( parent )
	, m_mux( QMutex::Recursive )
	, m_niippPoint( 0 )
	, m_pointKKlastInd( 0 )
	, m_ellipseCounter( 0 )
	, m_captureCircle(NULL)
    , m_netLength(0.1)
    , m_useCustomArea(false)
{
	m_view = pwWidget;
	m_pwWidget = pwWidget->getPwGis();

	connect(pwWidget, SIGNAL(signalApplyCross(int)), this, SLOT(onApplyCross(int)));

	m_styleManager = new MapStyleManager( m_pwWidget->mapProvider()->styleFactory() );
	m_objectsManager = m_pwWidget->mapProvider()->objectsManager();

	m_factory = new MapFeature::FeaturesFactory(m_view, m_pwWidget->mapProvider(), m_styleManager );

	m_niippPoint = m_factory->createNiippPoint();

	m_circleRadius = 0;
	m_circleChanged = false;

	m_bounds = m_pwWidget->mapProvider()->mapBounds();
	m_station = station;

	m_hyperboleTimer = new QTimer(this);
	connect( m_hyperboleTimer, SIGNAL(timeout()), this, SLOT(removeAllHyperbole()) );

	m_onePointTimer = new QTimer(this);
	connect( m_onePointTimer, SIGNAL(timeout()), this, SLOT(removeAllonePointTimer()) );
	m_onePointTimer->start(3000);

	/// \todo I don't know what is there for these timers, maybe it was some code for demo?
	m_uiTimer = new QTimer( this );
	connect( m_uiTimer, SIGNAL( timeout() ), this, SLOT( updateCircle() ) );
	m_uiTimer->setInterval( 100 );
	//m_uiTimer->start();

	m_interception = new ZInterception( this );
	QThread* thread = new QThread;
	connect( m_interception, SIGNAL( finished() ), thread, SLOT( quit() ) );
	connect( m_interception, SIGNAL( finished() ), m_interception, SLOT( deleteLater() ) );
	connect( thread, SIGNAL( finished() ), thread, SLOT( deleteLater() ) );
	m_interception->moveToThread( thread );
	thread->start();

	connect( &m_bplaRedrawTimer, SIGNAL( timeout() ), SLOT( redrawAllBpla()) );
	m_bplaRedrawTimer.start( 100 );

	connect( this, SIGNAL(friendBplaAdded(UavInfo)),
			 this, SLOT(addFriendBplaInternal(UavInfo)) );

	connect( this, SIGNAL(enemyBplaAdded(UavInfo,QVector<QPointF>,QVector<QPointF>)),
			 this, SLOT(addEnemyBplaInternal(UavInfo,QVector<QPointF>,QVector<QPointF>)) );

	connect( this, SIGNAL(singleMarkAdded(QByteArray)),
			 this, SLOT(addSingleMarkInternal(QByteArray)) );

	connect( this, SIGNAL(trajectoryKKAdded(QByteArray, int)),
			 this, SLOT(addTrajectoryKKInternal(QByteArray, int)) );

	connect( this, SIGNAL(bplaRemoved(Uav)),
			 this, SLOT(removeBplaInternal(Uav)) );

	connect( this, SIGNAL(niippPowerZoneUpdated(Niipp)),
			 this, SLOT(addNiipInternal(Niipp)) );

	connect( this, SIGNAL(interceptionAdded(int, int, QList<UavInfo>&, QList<UavInfo>&)),
			 this, SLOT(addPerehvatData(int, int, QList<UavInfo>&, QList<UavInfo>&)) );

	connect( this, SIGNAL(interceptionRemoved(int, int)),
			 this, SLOT(removeInterceptionData(int, int)) );

	connect( this, SIGNAL(hyperboleAdded(QByteArray,int, QColor)),
			 this, SLOT(addHyperboleInternal(QByteArray,int, QColor)) );

	connect( this, SIGNAL(signalAddStation(QByteArray)),
			 this, SLOT(addStationInternal(QByteArray)) );

	connect( this, SIGNAL(signalAddArea(QPointF,QPointF)),
			 this, SLOT(addAreaInternal(QPointF,QPointF)) );

	connect( this, SIGNAL(interceptionPointAdded(int, int, QPointF, float, float, int, float, float)),
			 this, SLOT(addInterceptionPointData(int, int, QPointF, float, float, int, float, float)) );

	connect(&m_objectsManager->events(), SIGNAL(featureClicked(QString,QString)), SLOT(onFeatureClicked(QString,QString)));

	connect(&m_objectsManager->events(), SIGNAL(featureAdded(QString,QString)), SLOT(onFeatureAdded(QString,QString)));

	connect(this, SIGNAL(onSolverClear()), this, SLOT(slotSolverClear()));

	connect(m_view, SIGNAL(signalApply(int)), this, SLOT(countCaptureApply(int)));
	connect(m_view, SIGNAL(signalClear()), this, SLOT(slotCaptureClear()));

	connect(pwWidget, SIGNAL(onShowBaseStation(double,double,QString)), this, SLOT(slotAddBaseStation(double, double, QString)));
	connect(pwWidget, SIGNAL(onClearBaseStation()), this, SLOT(slotClearBaseStation()));

    m_qLbl = new QLabel("txtQuadrant", m_pwWidget, Qt::Popup);

    m_tcpClientManager = nullptr;
}

MapClient1::~MapClient1()
{
	delete m_niippPoint;
	delete m_factory;
	delete m_styleManager;

	removeAll();
}

void MapClient1::init()
{
	m_mapLayers.clear();
	removeAll();

	// create marker layers
    addMarkerLayer( 0, "OP", tr( "PIPS" ) );

    //addMarkerLayer( 6, "Atlant", tr( "Atlant" ) );
    //addMarkerLayer( 7, "Atlant_target", tr( "Atlant target" ) );
    //addMarkerLayer( 8, "Grid", tr( "Grid" ) );
	addMarkerLayer( 9, "Checkpoints", tr( "Checkpoints" ) );
	addMarkerLayer( 10, "Interception_point", tr( "Interception point" ) );
    //addMarkerLayer( 11, "Civil_ships", tr( "Civil ships" ) );
    //addMarkerLayer( 12, "Diversion_points", tr( "Diversion points" ) );
    //addMarkerLayer( 13, "SPIP_DD", tr( "SPIP DD" ) );
	addMarkerLayer( 14, "Hyperbole", tr( "Hyperbole" ) );
	addMarkerLayer( 15, "HyperboleZone", tr( "Hyperbole-Zone" ) );
	addMarkerLayer( 16, "History", tr( "History" ) );
	addMarkerLayer( 17, "PointKK", tr( "Point" ) );
	addMarkerLayer( 18, "WorkArea", tr( "workArea" ) );
    addMarkerLayer( 19, "NetArea", tr( "NetArea" ) );

	addMarkerLayer( 101, "UAV_enemy_track_manual", tr( "UAV_enemy_track_manual" ) );
	addMarkerLayer( 100, "UAV_enemy_manual", tr( "UAV_enemy_manual" ) );

	addMarkerLayer( 103, "UAV_enemy_track_auto", tr( "UAV_enemy_track_auto" ) );
	addMarkerLayer( 102, "UAV_enemy_auto", tr( "UAV_enemy_auto" ) );

    //addMarkerLayer( 104, "UAV_enemy_single", tr( "UAV_enemy_single" ) );

	addMarkerLayer( 5, "UAV_track_KTR", tr( "UAV_track_KTR" ) );
	addMarkerLayer( 4, "UAV_track_autopilot", tr( "UAV_track_autopilot" ) );
	addMarkerLayer( 3, "UAV", tr( "UAV" ) );

	addMarkerLayer( 202, "ADSB", tr( "ADSB" ) );
	addMarkerLayer( 203, "ADSBtrack", tr( "ADSBtrack" ) );

    addMarkerLayer( 300, "StationsPVO", tr( "Stations PVO & REB" ) );


	showLayer( 8, false );
	showLayer( 9, false );
	showLayer( 14, false );
	showLayer( 15, false );
	showLayer( 17, false );
    showLayer( 19, false );

	// create styles for features
	m_styleManager->createStationStyle( m_mapLayers.value(0) )->apply();

	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(100), UAV_SOLVER_MANUAL_SOURCE, 1 )->apply();
	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(100), UAV_SOLVER_MANUAL_SOURCE, 2 )->apply();
	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(100), UAV_SOLVER_MANUAL_SOURCE, 3 )->apply();
	m_styleManager->createEnemyBplaTrackStyle( m_mapLayers.value(101), UAV_SOLVER_MANUAL_SOURCE )->apply();
	m_styleManager->createEnemyBplaErrorStyle( m_mapLayers.value(101), UAV_SOLVER_MANUAL_SOURCE )->apply();

	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(102), UAV_SOLVER_AUTO_SOURCE, 1 )->apply();
	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(102), UAV_SOLVER_AUTO_SOURCE, 2 )->apply();
	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(102), UAV_SOLVER_AUTO_SOURCE, 3 )->apply();
	m_styleManager->createEnemyBplaTrackStyle( m_mapLayers.value(103), UAV_SOLVER_AUTO_SOURCE )->apply();
	m_styleManager->createEnemyBplaErrorStyle( m_mapLayers.value(101), UAV_SOLVER_AUTO_SOURCE )->apply();

	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(104), UAV_SOLVER_SINGLE_1_SOURCE, 1 )->apply();
	m_styleManager->createEnemyBplaStyle( m_mapLayers.value(104), UAV_SOLVER_SINGLE_2_SOURCE, 1 )->apply();

	m_styleManager->createFriendBplaStyle( m_mapLayers.value(3) )->apply();
	m_styleManager->createFriendBplaTrackStyle( m_mapLayers.value(4) )->apply();
	m_styleManager->createFriendBplaSliceStyle( m_mapLayers.value(5) )->apply();

	m_styleManager->createPelengatorStyle( m_mapLayers.value(6) )->apply();
	m_styleManager->createPelengatorPointStyle( m_mapLayers.value(7) )->apply();
	m_styleManager->createGridStyle( m_mapLayers.value(8) )->apply();
	m_styleManager->createCheckPointStyle( m_mapLayers.value(9) )->apply();
	m_styleManager->createInterceptionStyle( m_mapLayers.value(10) )->apply();
	m_styleManager->createAisStyle( m_mapLayers.value(11) )->apply();
	m_styleManager->createAdsbStyle( m_mapLayers.value(202) )->apply();
	m_styleManager->createAdsbTrackStyle( m_mapLayers.value(203) )->apply();
	m_styleManager->createNiippPointStyle( m_mapLayers.value(12) )->apply();
	m_styleManager->createNiippStyle( m_mapLayers.value(13) )->apply();
	m_styleManager->createHyperboleStyle( m_mapLayers.value(14) )->apply();
	m_styleManager->createHistoryStyle( m_mapLayers.value(16) )->apply();
	//m_styleManager->createHistoryStyle( m_mapLayers.value(16) )->apply();

	for(int i = 1; i<11; i++) {
		m_styleManager->createHyperboleZoneStyle( m_mapLayers.value(15), i )->apply();
	}

    m_pvoSimpleStyle = m_styleManager->createStationPvoStyle(m_mapLayers.value(300), 0)->apply();
    m_pvoActiveStyle = m_styleManager->createStationPvoStyle(m_mapLayers.value(300), 1)->apply();
    m_rebSimpleStyle = m_styleManager->createStationRebStyle(m_mapLayers.value(300), 0)->apply();
    m_rebActiveStyle = m_styleManager->createStationRebStyle(m_mapLayers.value(300), 1)->apply();

	for(int i = 1; i<11; i++) {
		m_styleManager->createKKpointStyle( m_mapLayers.value(17), double((double)i/(double)10) )->apply();
	}
	m_styleManager->createWorkAreaStyle( m_mapLayers.value(18) )->apply();
    m_styleManager->createNetAreaStyle( m_mapLayers.value(19) )->apply();

	//addNiippLayer
    //m_pwWidget->mapProvider()->layerManager()->addVectorLayer( "NIIPP", tr("NIIPP") );

	connect( this, SIGNAL( aisAdded( QMap<int, QVector<QString> >) ),
			 this, SLOT( setAisData( QMap<int, QVector<QString> > ) ) );

	connect( this, SIGNAL( pelengUpdated( int, int, double, double, double ) ),
			 this, SLOT( addPeleng( int, int, double, double, double ) ) );

	connect( this, SIGNAL(adsbAdded(QByteArray)), this, SLOT(setAdsbData(QByteArray)) );
}

void MapClient1::addHyperboles(const QByteArray& data, int version, const QColor color)
{
	emit hyperboleAdded(data, version, color);
}

void MapClient1::DrawHyerboles(QList<QVector<QPointF>> list, QTime time, const QColor color)
{
	if ( m_hyperboleList.count() > list.count() ) {
		for ( int i = list.count(); i < m_hyperboleList.count(); i++ ) {
			m_hyperboleList[i]->removeFromMap();
		}
	}
	//    if ( m_hyperboleList.count() > 0 ) {
	//        for ( int i = 0; i < m_hyperboleList.count(); i++ ) {
	//            m_hyperboleList[i]->removeFromMap();
	//            delete m_hyperboleList.at(i);
	//        }
	//    }

	//    m_hyperboleList.clear();

	for(int z=0; z<100; z++) {

		for ( int i = 0; i < list.count(); ++i ) {
			if( i < m_hyperboleList.count() ) {
				m_hyperboleList[i]->updatePath( list[i], time );
			} else {
				MapFeature::Hyperbole* hyperbole;
				hyperbole = m_factory->createHyperbole( list[i], time, color );
				m_hyperboleList << hyperbole;
			}
		}

		if ( m_hyperboleList.count() > 0 ) {
			for ( int i = 0; i < m_hyperboleList.count(); i++ ) {
				m_hyperboleList[i]->removeFromMap();
				delete m_hyperboleList.at(i);
			}
		}

		m_hyperboleList.clear();

	}
}

void MapClient1::DrawHyerboles(const QList<QVector<QPointF> > &list,
							   const QList<QVector<QPointF> > &zone,
							   const QTime &time, const QColor &color)
{
	//    if ( m_hyperboleList.count() > list.count() ) {
	//        for ( int i = list.count(); i < m_hyperboleList.count(); i++ ) {
	//            m_hyperboleList[i]->removeFromMap();
	//        }
	//    }

	clearHyperbole();


	for ( int i = 0; i < list.count(); ++i ) {
		MapFeature::Hyperbole* hyperbole;
		hyperbole = m_factory->createHyperbole( list[i], zone[i], time, color );
		m_hyperboleList << hyperbole;
	}

}

void MapClient1::addHyperboleInternal(const QByteArray& data, int version, const QColor color )
{
	m_hyperboleTimer->start(HYPERBOLE_LIFE_TIME);

	if(version == 0) {
		QDataStream ds(data);

		double frequency;
		ds >> frequency;

		QTime time;
		ds >> time;

		QList<QVector<QPointF>> list;
		ds >> list;

		// hide no needed hyporboles
		DrawHyerboles(list, time, color);
	}
	//Parse features from new solver !
	else if(version == 1) {

		SolverProtocol::Packet_DataFromSolver_SolverSolution_PositionLines linesArray;
		if( !linesArray.ParseFromArray( data.data(), data.size() ) ) {
			return;
		}

		QList<QVector<QPointF>> list;
		QList<QVector<QPointF>> zoneList;

		for(int i = 0; i < linesArray.positionline_size(); i++) {
			QVector<QPointF> hyperbole;
			QVector<QPointF> zone;
			for(int j = 0; j < linesArray.positionline(i).linecoordinates_size(); j++) {
				QPointF point( linesArray.positionline(i).linecoordinates(j).lat(),
							   linesArray.positionline(i).linecoordinates(j).lon() );

				hyperbole.append(point);
			}

			for(int j = 0; j < linesArray.positionline(i).lineareacoordinates_size(); j++) {
				QPointF pointZone( linesArray.positionline(i).lineareacoordinates(j).lat(),
								   linesArray.positionline(i).lineareacoordinates(j).lon() );

				zone.append(pointZone);
			}
			//Aded accurancy
			zone.append(QPointF( linesArray.positionline(linesArray.positionline_size()-1).lineaccuracy(),
								 -10000));
			//===================
			list.append(hyperbole);
			zoneList.append(zone);
		}

		QDateTime time = QDateTime::fromMSecsSinceEpoch(linesArray.datetime());
		DrawHyerboles(list, zoneList, time.time(), color);
	}
}

void MapClient1::addStationInternal(QByteArray data)
{
	SolverProtocol::Detectors pkt;
	if( !pkt.ParseFromArray( data.data(), data.size() ) ) {
		return;
	}

	foreach (MapFeature::Station* station, m_stationList.values()) {
		station->removeFromMap();
		delete station;
	}

	m_stationList.clear();

    for(int i = 0; i<m_stationsArea.size(); i++) {
        m_pwWidget->removeObject( QString("AreaRect%1").arg(i) );
    }
    m_stationsArea.clear();

	for(int i = 0; i<pkt.detector_size(); i++ ) {
		MapFeature::Station* station;
		QString name = QString::fromStdString(  pkt.detector(i).detector_name() );
		QPointF pos(pkt.detector(i).coords().lon(),
					pkt.detector(i).coords().lat());

		station = m_factory->createStation( name, pos );
		m_stationList.insert( name, station );
        QVector<QPointF> pointAreaList;

        QSettings settings( "Map/map_stations_workArea.ini", QSettings::IniFormat );
        settings.setIniCodec( QTextCodec::codecForName("UTF-8") );
        double areaRadiusX = settings.value("areaRadiusX").toDouble();
        double areaRadiusY = settings.value("areaRadiusY").toDouble();
        bool useStationsArea = settings.value("useStationsArea", 0).toBool();

        if(useStationsArea) {
            QPointF pp1 = QPointF(pos.x()-areaRadiusX, pos.y()-areaRadiusY);
            QPointF pp2 = QPointF(pos.x()+areaRadiusX, pos.y()+areaRadiusY);

            pointAreaList.append(pp1);
            pointAreaList.append(pp2);
            QRectF poly;
            poly.setBottomLeft(pp1);
            poly.setTopRight(pp2);

            m_stationsArea.append(poly);
            if(poly.contains(pos)) {
                //log_debug("Ololo we contains!!!");
            } else {
                //log_debug(QString("no contains   xp  %1  yp  %2").arg(pos.x()).arg(pos.y()));
            }
        }

		station->updateMap();
	}


    int cnt = 0;
    foreach (QRectF rect, m_stationsArea) {
        m_pwWidget->addRectangle( QString("AreaRect%1").arg(cnt), rect.bottomLeft().x(),
                                  rect.bottomLeft().y(),
                                  rect.topRight().x(),
                                  rect.topRight().y(),
                                  "","", MAP_STYLE_NAME_NET_AREA);
        cnt++;
    }
}

QString MapClient1::getSquareName(double lon, double lat)
{
//    if(m_arealist.getCount() != 4) {
//        return QString("");
//    }

//    if(m_pwWidget->mapProvider()->layerManager()->isVisible(m_mapLayers.value( 19 ))) {

//        QPointF point1 = m_areaGeoList.at(0);
//        QPointF point2 = m_areaGeoList.at(1);

//        int xCnt = (point2.x() - point1.x()) / m_netLength;
//        int yCnt = (point2.y() - point1.y()) / m_netLength;

//        int xCntAll = (point2.x() - point1.x()) / m_netLength + 1;

//        if(point1.x() <= lon && point2.x() >= lon &&
//           point1.y() <= lat && point2.y() >= lat) {
//            int xpCnt = (lon - point1.x()) / m_netLength;
//            int ypCnt = (lat - point1.y()) / m_netLength;

//            QString xVal = QString(QChar(65+xpCnt%26));
//            QString xValAdd = QString::number(floor((double)xpCnt/26));

//            if(xCntAll > 26) {
//                xVal = xVal + xValAdd;
//            }

//            return QString(tr("Square: %1 : %2"))
//                    .arg(xVal)
//                    .arg(ypCnt);

//        }
//    } else {
//        return QString("");
//    }

    return QString("");
}

double getCorrectDegAngle(const double deg_1)
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

double DegreeToRadian(double angle)
{
	return M_PI * angle / 180.0;
}

double RadianToDegree(double angle)
{
	return 180.0 * angle / M_PI;
}

QPointF getPointByDistance(double lon1, double lat1, double dist, double azimuth)
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

void MapClient1::addSingleRoundArea(const QPointF center, int id)
{
	for(int i = 0; i<m_netLength; i++) {
		m_pwWidget->addCircle(QString("cir%1%2").arg(id).arg(i), center.x(), center.y(),
							  (i+1)*1444, /*QString("%1 km").arg(i)*/"", "", MAP_STYLE_NAME_NET_AREA);
	}

	for(int i = 0; i<36; i++) {
		QPointF endP = getPointByDistance(center.x(), center.y(), m_netLength*1000, i*10);
		m_pwWidget->addLine(QString("Aline%1%2").arg(id).arg(i),
									endP.x(), endP.y(),
									center.x(), center.y(),
									QString("%1").arg(i*10), "", MAP_STYLE_NAME_NET_AREA);
    }
}

void MapClient1::addEnemyBplaSim(double lat, double lon)
{
    UavInfo uav;
    uav.name = QString("Sim");
    uav.role = ENEMY_UAV_ROLE;
    uav.id = 1;
    uav.uavId = 1;
    uav.device = 1;
    uav.source = 1;
    uav.lat = lat;
    uav.lon = lon;
    uav.latStddev = 10;
    uav.lonStddev = 10;
    uav.alt = 1000;
    uav.speed = 100;
    uav.yaw = 0;
    uav.restTime = QTime(0,0,0,0);
    uav.statusId = 1;
    uav.dateTime = QDateTime::currentDateTime();
    uav.historical = false;
    addEnemyBpla(uav);
    log_debug("Sim1");

}

void MapClient1::addAreaNetInternal(QPointF point1, QPointF point2)
{
	addSingleRoundArea(point1, 1);
	addSingleRoundArea(point2, 2);
}

void MapClient1::addAreaInternal(QPointF point1, QPointF point2)
{
	QString name = QString("SolverWorkArea");

	QPointF point1Net;
	QPointF point2Net;
	if(m_areaGeoList.size() == 2) {
		point1Net = m_areaGeoList.at(0);
		point2Net = m_areaGeoList.at(1);
		int xCnt = (point2Net.x() - point1Net.x()) / m_netLength + 2;
		int yCnt = (point2Net.y() - point1Net.y()) / m_netLength + 1;
		for(int x = 0; x<xCnt+2; x++) {
			m_pwWidget->removeObject(QString("x%1").arg(x));
		}

		for(int y = 0; y<yCnt+2; y++) {
			m_pwWidget->removeObject(QString("y%1").arg(y));
		}
	}

	point1Net = point1;
	point2Net = point2;
	int inc = 1;
	if(!m_useCustomArea) {
		m_areaGeoList.clear();
		m_areaGeoList.append(point1);
		m_areaGeoList.append(point2);
	} else if(m_areaGeoList.size() == 2){
		point1Net = m_areaGeoList.at(0);
		point2Net = m_areaGeoList.at(1);
		inc = 2;
	}

	m_arealist.clear();

	m_arealist.append( new PwGisLonLat(point1.x(), point1.y()) );
	m_arealist.append( new PwGisLonLat(point1.x(), point2.y()) );
	m_arealist.append( new PwGisLonLat(point2.x(), point2.y()) );
	m_arealist.append( new PwGisLonLat(point2.x(), point1.y()) );


//    int xCnt = (point2Net.x() - point1Net.x()) / m_netLength + inc;
//    int yCnt = (point2Net.y() - point1Net.y()) / m_netLength + 1;

//    double lonCnt = point1Net.x();
//    double latCnt = point1Net.y();

//    int charCnt = -1;
//    for(int x = 0; x < xCnt; x++) {

//        QString val = QChar(65+x%26);
//        if(!(x%26)) {
//            charCnt++;
//        }

//        if(xCnt > 26) {
//            val = val + QString::number(charCnt);
//        }

//        m_pwWidget->addLine( QString("x%1").arg(x), lonCnt, point1Net.y(), lonCnt, point2Net.y(),
//                             QString(val),"", MAP_STYLE_NAME_NET_AREA);

//        lonCnt += m_netLength;
//    }

//    for(int y = 0; y < yCnt; y++) {
//        m_pwWidget->addLine( QString("y%1").arg(y), point1Net.x(), latCnt, point2Net.x(), latCnt,
//                             QString::number(y), "", MAP_STYLE_NAME_NET_AREA);

//        latCnt += m_netLength;
//    }

	m_pwWidget->addPolygon( name, &m_arealist, "", "", MAP_STYLE_NAME_WORK_AREA);
}


void MapClient1::addStation(const QByteArray& data)
{
	emit signalAddStation(data);
}

void MapClient1::addWorkArea(const QPointF &point1, const QPointF &point2)
{
	emit signalAddArea(point1, point2);
}

void MapClient1::clearSolver1()
{
	emit onSolverClear();
}

void MapClient1::removeAllHyperbole()
{
	for ( int i = 0; i < m_hyperboleList.count(); i++ ) {
		m_hyperboleList[i]->removeFromMap();
	}
}

void MapClient1::removeAllonePointTimer()
{
	foreach (MapFeature::EnemyBpla* bla, m_enemyBplaList) {
		if( bla->getStyleName() == MapStyleManager::getEnemyBplaStyleName(104, 1) ) {
			bla->removeFromMap();
			//delete bla;
			m_enemyBplaList.remove(m_enemyBplaList.key(bla));
			//m_enemyBplaList.clear();
		}
	}
}

void MapClient1::showLayer( int index, bool state )
{
	if ( m_mapLayers.contains( index ) ) {
		m_pwWidget->mapProvider()->layerManager()->setLayerVisibility( m_mapLayers.value( index ), state );
	}
}

void MapClient1::addFriendBpla(const UavInfo& uav)
{
	emit friendBplaAdded( uav );
}

void MapClient1::addEnemyBpla(const UavInfo& uav,
							  const QVector<QPointF>& tail, const QVector<QPointF>& tailStdDev)
{
	emit enemyBplaAdded( uav, tail, tailStdDev );
}

void MapClient1::addSingleMark( const QByteArray& uav )
{
	emit singleMarkAdded(uav);
}

void MapClient1::addTrajectoryKK(const QByteArray &uav, const int source)
{
	emit trajectoryKKAdded(uav, source);
}

void MapClient1::removeBpla(const Uav& uav)
{
	emit bplaRemoved( uav );
}

void MapClient1::addAis( QMap<int, QVector<QString> > vec )
{
	emit aisAdded( vec );
}

void MapClient1::addAdsb(QByteArray data)
{
	emit adsbAdded(data);
}

void MapClient1::updatePeleng( int id, int idPost, double lat, double lon, double direction )
{
	emit pelengUpdated( id, idPost, lat, lon, direction );
}

void MapClient1::removeAll()
{
	clearObjectsList( MapFeature::Ais, m_aisList );
	clearObjectsList( MapFeature::FriendBpla, m_friendBplaList );
	clearObjectsList( MapFeature::EnemyBpla, m_enemyBplaList );
	clearObjectsList( MapFeature::Niipp, m_niippList );
	clearObjectsList( MapFeature::Pelengator, m_pelengatorList );
	clearObjectsList( MapFeature::PelengatorPoint, m_pelengatorPointsList );
	clearObjectsList( MapFeature::Interception, m_interceptionList );
	clearObjectsList( MapFeature::Station, m_stationList );
	clearObjectsList( MapFeature::CheckPoint, m_checkPointsList );
    clearObjectsList( MapFeature::Hyperbole, m_hyperboleList );
}

void MapClient1::mapClicked(double lon, double lat)
{
}

void MapClient1::mapMoved(double lon, double lat)
{


    QString name = getSquareName(lon, lat);
    emit onSquare(name);
}

void MapClient1::setTcpClientManager(TcpClientManager *manager)
{
    m_tcpClientManager = manager;

    foreach (MapFeature::PvoFeature* feature, m_pvoList.values()) {
        feature->setTcpClientManager(m_tcpClientManager);
	}
}

void MapClient1::showHyperbole()
{
	showLayer( 14, true );
}

void MapClient1::addInterception(int blaId, int bplaId, QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList )
{
	emit interceptionAdded( blaId, bplaId, blaInfoList, bplaInfoList );
}

void MapClient1::removeInterception(int blaId, int bplaId )
{
	emit interceptionRemoved( blaId, bplaId );
}

void MapClient1::addPerehvatPoint(int blaId, int bplaId, QPointF coord,
								  float hgt, float radius, int time, float intcCourse, float intcSpeed)
{
	emit interceptionPointAdded( blaId, bplaId, coord, hgt, radius,
								 time, intcCourse, intcSpeed );
}

void MapClient1::removeNiippPoint()
{
	if(m_niippPoint != 0) {
		delete m_niippPoint;
	}

	m_niippPoint = 0;
}

void MapClient1::readCheckLinesFromFile(QString fileName)
{
	QSettings settings( fileName, QSettings::IniFormat );
	settings.setIniCodec( QTextCodec::codecForName("UTF-8") );

	QStringList childKeys = settings.childGroups();
	foreach( const QString &childKey, childKeys ) {
		settings.beginGroup( childKey );

		QVariant name = settings.value( "Name" );
		QVariant lat1 = settings.value( "Latitude1" );
		QVariant lon1 = settings.value( "Longitude1" );
		QVariant lat2 = settings.value( "Latitude2" );
		QVariant lon2 = settings.value( "Longitude2" );

		if( name.isNull() || !name.canConvert( QVariant::String ) ) continue;
		if( lat1.isNull() || !lat1.canConvert( QVariant::Double ) ) continue;
		if( lon1.isNull() || !lon1.canConvert( QVariant::Double ) ) continue;
		if( lat2.isNull() || !lat2.canConvert( QVariant::Double ) ) continue;
		if( lon2.isNull() || !lon2.canConvert( QVariant::Double ) ) continue;

		Pw::Gis::Path* tmpPath = m_pwWidget->mapProvider()->objectsFactory()->createPath();
		tmpPath->points()->append(new PwGisLonLat(lon1.toDouble(), lat1.toDouble()));
		tmpPath->points()->append(new PwGisLonLat(lon2.toDouble(), lat2.toDouble()));
		tmpPath->addStyleByName(MAP_STYLE_NAME_CHECK_POINTS);

		tmpPath->updateMap();

		settings.endGroup();
	}
}

void MapClient1::readCheckNetFromFile(QString fileName)
{
    QSettings settings( fileName, QSettings::IniFormat );
    settings.setIniCodec( QTextCodec::codecForName("UTF-8") );

    m_netLength = settings.value("length").toDouble();
    m_useCustomArea = settings.value("useCustomArea", 0).toBool();

    QPointF point1;
    point1.setX(settings.value("leftBottomX").toDouble());
    point1.setY(settings.value("leftBottomY").toDouble());

    QPointF point2;
    point2.setX(settings.value("rightTopX").toDouble());
    point2.setY(settings.value("rightTopY").toDouble());

    m_areaGeoList.append(point1);
    m_areaGeoList.append(point2);

	addAreaNetInternal(point1, point2);
}

void MapClient1::readCheckPointsFromFile(QString fileName)
{
	QSettings settings( fileName, QSettings::IniFormat );
	settings.setIniCodec( QTextCodec::codecForName("UTF-8") );

	QStringList childKeys = settings.childGroups();
	foreach( const QString &childKey, childKeys ) {
		settings.beginGroup( childKey );

		QVariant name = settings.value( "Name" );
		QVariant lat = settings.value( "Latitude" );
		QVariant lon = settings.value( "Longitude" );

		if( name.isNull() || !name.canConvert( QVariant::String ) ) continue;
		if( lat.isNull() || !lat.canConvert( QVariant::Double ) ) continue;
		if( lon.isNull() || !lon.canConvert( QVariant::Double ) ) continue;

		MapFeature::CheckPoint *p = m_factory->createCheckPoint(
					name.toString(), QPointF( lon.toDouble(), lat.toDouble() ) );

		p->updateMap();
		m_checkPointsList << p;

		settings.endGroup();
	}
}

void MapClient1::setPoint()
{
	QString mapObjectsSettingFile = QCoreApplication::applicationDirPath();
	mapObjectsSettingFile.append( "/Map/map_objects.ini" );

	// read settings for generated (positions)
	readStationsFromFile( mapObjectsSettingFile );

	QString mapPointsSettingFile = QCoreApplication::applicationDirPath();
	mapPointsSettingFile.append( "/Map/map_points.ini" );

	readCheckPointsFromFile( mapPointsSettingFile );

	QString mapLinesSettingFile = QCoreApplication::applicationDirPath();
	mapLinesSettingFile.append( "/Map/map_lines.ini" );

	readCheckLinesFromFile( mapLinesSettingFile );

    QString mapNetSettingFile = QCoreApplication::applicationDirPath();
    mapNetSettingFile.append( "/Map/map_net.ini" );

    readCheckNetFromFile( mapNetSettingFile );
}

void MapClient1::addMarkerLayer( int id, const QString& layerId,
								 const QString& name )
{
	m_pwWidget->mapProvider()->layerManager()->addMarkerLayer( layerId, name );
	m_mapLayers.insert( id, layerId );
}

/// set map centred to point
void MapClient1::centerMap()
{
	m_bounds->setMapCenter( m_station->longitude, m_station->latitude );
}

void MapClient1::justifyMap()
{
	int h = m_pwWidget->maximumHeight();
	int w = m_pwWidget->maximumWidth();
	m_bounds->zoomMapTo( 0, 0, w, h );
}

void MapClient1::addPerehvatData( int bla_id, int bpla_id, QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList )
{
	m_mapBattle.insert( bla_id, bpla_id );
	QByteArray target;
	QByteArray per;
	QDataStream ds1(&per, QIODevice::WriteOnly);
	QDataStream ds(&target, QIODevice::WriteOnly);
	UavInfo tempInfo;


    bool finded = false;
    foreach( MapFeature::FriendBpla* bpla, m_friendBplaList )  {
        if(bpla->getIntId() == bla_id) {
            tempInfo = bpla->getInfo();
            finded = true;
            break;
        }
    }


    if(!finded) {
        tempInfo = blaInfoList.last();
    }

	QPointF point1;
    point1.setY(tempInfo.lat);
    point1.setX(tempInfo.lon);
//    point1.setY(45.5);
//    point1.setX(34.1);
    ds1 << point1;
    double alt1 = /*tempInfo.alt*/1;
	ds1 << alt1;
    double speed1 = tempInfo.speed;
	ds1 << speed1;
	double course1 = tempInfo.yaw;
	ds1 << course1;
	//?
	int state1 = 1; //set by default. Don't need in interception counting
	ds1 << state1;


    finded = false;
    foreach( MapFeature::EnemyBpla* bpla, m_enemyBplaList )  {

        log_debug(QString("OUR ID : %1").arg(bpla->getIntId()));

        if(bpla->getIntId() == bpla_id) {
            tempInfo = bpla->getInfo();
            finded = true;
            break;
        }
    }

    if(!finded) {
        tempInfo = bplaInfoList.last();
    }
	int time = 1; //set by default
	ds << time;
	int state = 1; //set by default
	ds << state;
	QPointF sko;
    sko.setY(tempInfo.lat);
    sko.setX(tempInfo.lon);
//    sko.setY(45.5);
//    sko.setX(33.5);
	ds << sko;
	QVector<QPointF> track;
	track.append(sko);
	ds << track;
    double speed = tempInfo.speed;
	ds << speed;
    double alt = tempInfo.alt;
	ds << alt;
	double bearing = tempInfo.yaw;
	ds << bearing;

	m_interception->set(bla_id, bpla_id, per, target);
}

//for timer
void MapClient1::updateCircle()
{
	if ( !m_circleChanged ) {
		return;
	}
	m_circleChanged = false;

	m_pwWidget->addClassicPolygon( "c1",  30.531368, 60.074592,
								   m_circleRadius*1000, 40, 0, "ОП1 Гроза", "", "yellow selectAndDrag" );
}

void MapClient1::addNiippPoint( const QPointF& point )
{
	if( !m_niippPoint ) {
		m_niippPoint = m_factory->createNiippPoint( );
	}

	m_niippPoint->setPosition( point );
	m_niippPoint->updateMap();
}

void MapClient1::updateNiippPowerZone(const Niipp& niipp)
{
    emit niippPowerZoneUpdated( niipp );
}

void MapClient1::removeInterceptionData( int friendBplaId, int enemyBplaId )
{
	m_mapBattle.remove( friendBplaId );

	//removing interception map feature area from map
	QString interceptionKey = QString( "%1-%2" ).arg( friendBplaId ).arg( enemyBplaId );
	if(m_interceptionList.contains(interceptionKey)) {
		delete m_interceptionList.take( interceptionKey );
	}
}

void MapClient1::addFriendBplaInternal(const UavInfo& uav)
{
	QString id;

	if(uav.historical) {
		id = UavModel::getExtendedIdHist( uav );
	} else {
		id = UavModel::getExtendedId( uav );
	}

	MapFeature::FriendBpla* bpla = m_friendBplaList.value( id, NULL );

	m_uavKnownSources << uav.source;

	switch( uav.source ) {
	// if we get data about UAV from slices source...
	case UAV_SLICES_SOURCE:
		// and UAV exists, update slice track
		if( bpla != NULL ) {
			bpla->setSlice( QPointF( uav.lon, uav.lat ) );
			bpla->setInfo(uav);
		} else {
			// else create new one
			bpla = m_factory->createFriendBpla( uav );
			m_friendBplaList.insert( id, bpla );
		}

		return;

	case UAV_AUTOPILOT_SOURCE:
		if( bpla != NULL ) {
			// update, if BPLA already added and position changed
			bpla->update( uav );
		} else {
			// else create new one
			bpla = m_factory->createFriendBpla( uav );
			m_friendBplaList.insert( id, bpla );
		}

		return;
	}
}

void MapClient1::addEnemyBplaInternal(const UavInfo& uav,
									  const QVector<QPointF>& tail,
									  const QVector<QPointF>& tailStdDev)
{
	QString id;
	if(uav.historical) {
		id = UavModel::getExtendedIdHist( uav );
	} else {
		id = UavModel::getExtendedId( uav );
	}
	MapFeature::EnemyBpla* bpla = m_enemyBplaList.value( id, NULL );

	m_uavKnownSources << uav.source;

	if( bpla != NULL ) {
		bpla->update( uav );
	} else {
		bpla = m_factory->createEnemyBpla( uav );
		m_enemyBplaList.insert( id, bpla );
	}

	//Do not set tail temporally
	if( !uav.historical ) {
		bpla->setTail( tail, tailStdDev );

        if(m_tcpClientManager) {
                        m_tcpClientManager->sendEnemyToServer(uav);
                        m_tcpClientManager->sendEnemyToClient(uav);
        }

        foreach (MapFeature::PvoFeature* val, m_pvoList.values()) {
            val->setEnemyPoint(id, uav);
        }
	}

	bpla->updateMap();
}

void MapClient1::setBoardExtraInfo(int val)
{
	foreach (MapFeature::EnemyBpla* bpla, m_enemyBplaList) {
		bpla->setExtraInfo(val);
	}
}

void MapClient1::addSingleMarkInternal(QByteArray data)
{
	SolverProtocol::Packet_DataFromSolver_SolverSolution_SingleMarks sMsg;
	sMsg.ParseFromArray(data.data(), data.size());
	UavInfo uav;
	uav.dateTime = QDateTime::fromMSecsSinceEpoch( sMsg.datetime() );

    //clearEllipse();
	//    if(m_pointKKlastInd > 300) {
	//        clearKKPoint();
	//    }

	for(int i = 0; i<sMsg.singlemark_size(); i++) {
		uav.id = i;
		uav.alt = sMsg.singlemark(i).coordinates().alt();
		uav.lat = sMsg.singlemark(i).coordinates().lat();
		uav.lon = sMsg.singlemark(i).coordinates().lon();
		//addEnemyBpla(uav);

		//        m_pwWidget->addPoint(QString("KKpoint_%1").arg(m_pointKKlastInd),
		//                             uav.lon, uav.lat, "", "", MAP_STYLE_NAME_POINTKK);

		m_factory->createKKpoint(m_pwWidget, QPointF(uav.lon, uav.lat), QDateTime::currentDateTime());

//		PwGisPointList list;
//		for(int k=0; k < sMsg.singlemark(i).error_ellips().point_size(); k++ ) {
//			SolverProtocol::ErrorEllips_LatLon pp = sMsg.singlemark(i).error_ellips().point(k);

//			PwGisLonLat *ll = new PwGisLonLat(pp.lon(), pp.lat());

//			list.addLonLat(ll);
//		}

//		m_pwWidget->addPolygon( QString("ERR_Ellipse_%1").arg(m_ellipseCounter),
//								&list, "", "",  MAP_STYLE_NAME_WORK_AREA );

		m_ellipseCounter++;
		//m_pointKKlastInd++;
	}
}

void MapClient1::addTrajectoryKKInternal(QByteArray data, int source)
{

	SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory sol;
	sol.ParseFromArray( data.data(), data.size() );

	UavInfo uav;
	uav.name = QString::fromStdString(sol.targetid());
	uav.id = sol.central_frequency();
	uav.lat = sol.motionestimate(sol.motionestimate_size()-1).coordinates().lat();
	uav.lon = sol.motionestimate(sol.motionestimate_size()-1).coordinates().lon();
	uav.latStddev = sol.motionestimate(sol.motionestimate_size()-1).coordinates_acc().lat_acc();
	uav.lonStddev = sol.motionestimate(sol.motionestimate_size()-1).coordinates_acc().lon_acc();
	uav.alt = sol.motionestimate(sol.motionestimate_size()-1).coordinates().alt();
	uav.yaw = sol.motionestimate(sol.motionestimate_size()-1).relativebearing();
	uav.source = source;
	uav.statusId = sol.motionestimate(sol.motionestimate_size()-1).state();

    QString targetId = UavModel::getExtendedId( uav );

    log_debug(targetId + ".....");

    foreach (QRectF area, m_stationsArea) {
        if(area.contains(QPointF(uav.lon, uav.lat))) {
            log_debug("Contains!!!");
            if( m_enemyBplaList.contains( targetId ) ) {
                m_enemyBplaList.value(targetId)->removeFromMap();
            }
            return;
        }
    }


	QVector<QPointF> tail;

	for(int i = 0; i<sol.motionestimate_size(); i++) {
		tail.append(QPointF(sol.motionestimate(i).coordinates().lat(), sol.motionestimate(i).coordinates().lon()));
	}

	addEnemyBplaInternal(uav, tail, QVector<QPointF>());

	//clearTrajEllipse(targetId);

	//m_trajEllipseCounter.append(targetId);
	PwGisPointList list;
	for(int i = 0; i<sol.error_ellips().point_size(); i++ ) {
		SolverProtocol::ErrorEllips_LatLon pp = sol.error_ellips().point(i);

		PwGisLonLat *ll = new PwGisLonLat(pp.lon(), pp.lat());

		list.addLonLat(ll);
	}

	if( m_enemyBplaList.contains( targetId ) ) {
		//clearTrajEllipse( id );
		m_enemyBplaList.value(targetId)->setEllipse(list);
	}

}

void MapClient1::clearKKPoint()
{
	for(int k = 0; k < m_pointKKlastInd; k++) {
		m_pwWidget->removeObject(QString("KKpoint_%1").arg(k));
	}

	m_pointKKlastInd = 0;
}

void MapClient1::clearEllipse()
{
	for(int k = 0; k < m_ellipseCounter+2; k++) {
		m_pwWidget->removeObject(QString("ERR_Ellipse_%1").arg(k));
	}

	m_ellipseCounter = 0;
}

void MapClient1::clearTrajEllipse(QString id)
{
	m_trajEllipseCounter.removeOne(id);
	m_pwWidget->removeObject(QString("ERR_trajectory_Ellipse_%1").arg(id));

	clearEllipse();
}

void MapClient1::clearHyperbole()
{
	if ( m_hyperboleList.count() > 0 ) {
		for ( int i = 0; i < m_hyperboleList.count(); i++ ) {
			m_hyperboleList[i]->removeFromMap();
			delete m_hyperboleList.at(i);
		}
	}

	m_hyperboleList.clear();
}

void MapClient1::slotSolverClear()
{
	clearEllipse();
	clearKKPoint();
	clearHyperbole();
}

QPointF MapClient1::drawAim(QPointF pos, int angle)
{
	double BB = 34.023650 - 34.153769;
	double A = (46.018133-45.819489)/BB;
	double B = 1;
	double C = (34.153769*45.819489 - 34.023650*46.018133)/BB;

	double A1 = -tan((double)angle);
	double B1 = 1;
	double C1 = tan((double)angle)*pos.x()-pos.y();

	QPointF res;

	res.setX(  -(  (C*B1-C1*B)/(A*B1-A1*B)  )   );
	double tmp;
	if(angle>180) {
		res.setX( res.x() - 0.2*(1-modf(angle/90, &tmp)) );
	} else {
		res.setX( res.x() + 0.2*(1-modf(angle/90, &tmp)) );
	}

	res.setY(  -(  (A*C1-A1*C)/(A*B1-A1*B)  )   );

	log_debug(QString("aimX: %1").arg(res.x()));
	log_debug(QString("aimY: %1").arg(res.y()));

	return res;
}

void MapClient1::countCaptureApply(int aim)
{
	bool isFound = false;
	QString keyBla;
	foreach (QString key, m_enemyBplaList.keys()) {
		int numStart = key.indexOf("_");
		int numEnd = key.indexOf("-");

		QString valid = key.mid(numStart+1, numEnd-numStart-1).toInt();

		int id = valid.toInt();
		id = 1;

		log_debug(QString("start %1 end %2 str %3   resultId  %4").arg(numStart).arg(numEnd).arg(valid).arg(id));

		if(id == aim) {
			isFound = true;
			keyBla = key;
			break;
		}
	}

	if(!isFound) {
		return;
	}

	QPointF point(m_enemyBplaList.value(keyBla)->position().x(),
				  m_enemyBplaList.value(keyBla)->position().y());

	QPointF aimPoint = drawAim(point, m_enemyBplaList.value(keyBla)->cource());

	m_captureCircle = m_pwWidget->mapProvider()->objectsFactory()->createCircle();
	m_captureCircle->setName(tr("Capture aim %1").arg(aim));

	m_captureCircle->setOriginPoint(new PwGisLonLat(aimPoint.x(), aimPoint.y()));
	m_captureCircle->setRadius(5000);
	m_captureCircle->addStyleByName(MAP_STYLE_NAME_WORK_AREA);

	m_captureCircle->updateMap();
}

void MapClient1::slotCaptureClear()
{
    foreach(MapFeature::Interception* interc, m_interceptionList) {
        interc->removeFromMap();
    }
    m_interceptionList.clear();

	if(m_captureCircle) {
		m_captureCircle->removeFromMap();
        //removeBpla();
	}

	m_captureCircle = NULL;
}

void MapClient1::removeBplaInternal(const Uav& uav)
{
	foreach(uint source, m_uavKnownSources) {
		QString id = UavModel::getExtendedId( uav, source );

		if( m_friendBplaList.contains( id ) ) {
			delete m_friendBplaList.take( id );
		}

		if( m_enemyBplaList.contains( id ) ) {
			clearTrajEllipse( id );
			delete m_enemyBplaList.take( id );
		}
	}

	clearEllipse();
}

//add AIS
void MapClient1::setAisData( QMap<int, QVector<QString> > data )
{
	QMap<QString, MapFeature::Ais*> newAisList;

	QMap<int, QVector<QString> >::iterator it;
	for( it = data.begin(); it != data.end(); ++it ) {
		bool doubleOK;

		QVector<QString> vec = it.value();

		double lon = vec.at(1).toDouble( &doubleOK );
		if( !doubleOK ) {
			log_warning( QString( "Wrong longitude (%1) for AIS %2" ).arg( vec.at(1), vec.at(0) ) );
			continue;
		}

		double lat = vec.at(2).toDouble( &doubleOK );
		if( !doubleOK ) {
			log_warning( QString( "Wrong latitude (%1) for AIS %2" ).arg( vec.at(2), vec.at(0) ) );
			continue;
		}

		double course = vec.at(5).toDouble( &doubleOK );
		if( !doubleOK ) {
			log_warning( QString( "Wrong course (%1) for AIS %2" ).arg( vec.at(5), vec.at(0) ) );
			continue;
		}

		QString name = vec.at(0);
		QPointF position( lon, lat );

		// try to pass or update this AIS, if it exists
		MapFeature::Ais* ais = m_aisList.value( name, NULL );
		if( ais != NULL ) {
			m_aisList.remove( name );
			newAisList.insert( name, ais );

			if( ais->name() == name && ais->position() == position && ais->course() == course ) {
				continue;
			}

			// if new data is not same, update it
			ais->setName( name );
			ais->setPosition( position );
			ais->setCourse( course );
			ais->updateMap();
			continue;
		}

		ais = m_factory->createAis( name, position, course );
		newAisList.insert( name, ais );
	}

	// now remove remains AIS from old list
	clearObjectsList( MapFeature::Ais, m_aisList );

	m_aisList = newAisList;
}

void MapClient1::setAdsbData(QByteArray data)
{
	ADSBData::Packet packet;
	packet.ParseFromArray( data.data(), data.size() );

	if(packet.board_size() <= 0) {
		return;
	}

	foreach (ADSBData::Packet_Board var, packet.board()) {

		QString id;
		double lon;
		double lat;
		double yaw = 0;

		id = QString::fromStdString(var.id());
		lon = var.lon();
		lat = var.lat();

		if(var.has_yaw()) {
			yaw = var.yaw();
		}


		// add or update pelengator
		MapFeature::ADSBPlaneFeature* p = m_adsbList.value( id, NULL );
		if( p != NULL ) {
			if(lon > 180 || lon < -180) {
				return;
			}
			if(lat > 90 || lat < -90) {
				return;
			}


			if( lat != lat || lon != lon) {
				return;
			}

			p->update(QPointF(lon, lat), yaw);
			p->updateMap();
		} else {
			if(lon > 180 || lon < -180) {
				return;
			}
			if(lat > 90 || lat < -90) {
				return;
			}

			if( lat != lat ||
					lon != lon) {
				return;
			}

			p = m_factory->createAdsbPlane( id, QPointF(lon, lat) );
			p->updateMap();

			m_adsbList.insert( id, p );

			connect(p, SIGNAL(onFeatureRemove(QString)), this, SLOT(removeAdsb(QString)));
		}

	}
}

void MapClient1::removeAdsb(QString id)
{
	MapFeature::ADSBPlaneFeature* p = m_adsbList.value(id);

	if(p) {
		delete p;
	}
	m_adsbList.remove(id);
}

void MapClient1::onFeatureClicked(QString id, QString type)
{
	foreach (MapFeature::ADSBPlaneFeature* feature, m_adsbList) {
		QString tt = feature->getMapId();
		if(feature->getMapId() == id) {
			feature->onClicked();
			break;
		}
	}
}

void MapClient1::onFeatureAdded(QString id, QString type)
{
}

void MapClient1::slotAddBaseStation(double lon, double lat, QString name)
{
	if(lon == 0 && lat == 0) {
		return;
	}

	Pw::Gis::Marker* marker = m_pwWidget->mapProvider()->objectsFactory()->createMarker();
	marker->setPosition(new PwGisLonLat(lon, lat));
	marker->setName(name);
	marker->updateMap();
	m_baseStationMarkerList.append(marker);
}

void MapClient1::slotClearBaseStation()
{
	foreach (Pw::Gis::Marker* marker, m_baseStationMarkerList) {
		marker->removeFromMap();
		delete marker;
	}

	m_baseStationMarkerList.clear();
}

void MapClient1::onApplyCross(int val)
{
	QList<UavInfo> l1;
	QList<UavInfo> l2;

	foreach (QString id, m_enemyBplaList.keys()) {
		log_debug(id);
	}

	foreach (QString id, m_friendBplaList.keys()) {
		log_debug(id);
	}

	int idEnemy = 0;
	int idFriend = 0;

	idEnemy = val >> 16;
	idFriend = val & 0xFFFF;

	QList<UavInfo> enemyInfo;
	QList<UavInfo> friendInfo;

	bool isEnemy = false;
	bool isFriend = false;

	foreach (QString id, m_enemyBplaList.keys()) {
		if(id.contains(QString::number(idEnemy))) {
			enemyInfo.append(m_enemyBplaList.value(id)->getInfo());
			isEnemy = true;
		}
	}

	foreach (QString id, m_friendBplaList.keys()) {
		if(id.contains(QString::number(idFriend))) {
			friendInfo.append(m_friendBplaList.value(id)->getInfo());
			isFriend = true;
		}
	}

	if( isEnemy &&
        isFriend ) {
		addPerehvatData(idFriend, idEnemy, friendInfo, enemyInfo);
    } else {
        friendInfo.append(UavInfo());
        enemyInfo.append(UavInfo());
        addPerehvatData(idFriend, idEnemy, friendInfo, enemyInfo);
    }
}

void MapClient1::readStationsFromFile(QString fileName)
{
	return; // Do not read from file. Stations from solver

	QTextCodec* codec = QTextCodec::codecForName( "UTF-8" );
	QSettings stationSettings( fileName, QSettings::IniFormat );

	stationSettings.setIniCodec( codec );

	QString name;
	double lat;
	double lon;
	QPointF position;

	QStringList childKeys = stationSettings.childGroups();
	foreach( const QString &childKey, childKeys ) {
		stationSettings.beginGroup( childKey );

		name = stationSettings.value( "Name", 0 ).toString();
		lat = stationSettings.value( "Latitude", "0" ).toDouble();
		lon = stationSettings.value( "Longitude", "0" ).toDouble();

		position = QPointF( lon, lat );

		if( name != tr( "Ingur" ) ) {
			MapFeature::Station* station = m_stationList.value( name, NULL );

			if( station = NULL ) {
				station->setPosition( position );
			} else {
				station = m_factory->createStation( name, position );
				m_stationList.insert( name, station );
			}

			station->updateMap();
		}

		stationSettings.endGroup();
	}
}

void MapClient1::addPeleng(int id, int idPost, double lat, double lon, double direction)
{
	Q_UNUSED( id );
	QPointF position( lon, lat );

	// add or update pelengator
	MapFeature::Pelengator* p = m_pelengatorList.value( idPost, NULL );
	if( p != NULL ) {
		p->update( position, direction );
	} else {
		p = m_factory->createPelengator( idPost, position, direction );
		p->updateMap();

		m_pelengatorList.insert( idPost, p );
	}
}

void MapClient1::updateStationPowerZone(const NStation &niipp)
{
    // add or update NIIPP
    MapFeature::PvoFeature* niippFeature = m_pvoList.value( niipp.getID(), NULL );
    if( niippFeature != NULL ) {
        niippFeature->update( niipp );
    } else {
        niippFeature = m_factory->createPvoStation( niipp,m_pvoSimpleStyle, m_pvoActiveStyle,
                                                    m_rebSimpleStyle, m_rebActiveStyle);
        niippFeature->setTcpClientManager(m_tcpClientManager);
        niippFeature->updateMap();

        m_pvoList.insert( niipp.getID(), niippFeature );
    }
}

void MapClient1::removeStationPowerZone(const NStation &niipp)
{
    // add or update NIIPP

    MapFeature::PvoFeature* niippFeature = m_pvoList.value( niipp.getID(), NULL );
    if( niippFeature != NULL ) {
        m_pvoList.remove(niipp.getID());
        niippFeature->removeFromMap();
        delete niippFeature;
    }
}

void MapClient1::addNiipInternal( const Niipp& niipp )
{
	// add or update NIIPP
	MapFeature::Niipp* niippFeature = m_niippList.value( niipp.getId(), NULL );
	if( niippFeature != NULL ) {
		niippFeature->update( niipp );
	} else {
		niippFeature = m_factory->createNiipp( niipp );
		niippFeature->updateMap();

		m_niippList.insert( niipp.getId(), niippFeature );
	}
}

void MapClient1::addInterceptionPointData( int friendBplaId, int enemyBplaId, QPointF position,
										   float height, float radius, int time, float course, float speed )
{
	Q_UNUSED( time );

    if(time == 999999) {
        QMessageBox::warning(0, tr("Capture"), tr("Capture in hour is impossible"), QMessageBox::Ok);
    }

	QString key = QString( "%1-%2" ).arg( friendBplaId ).arg( enemyBplaId );

	MapFeature::Interception* intc = m_interceptionList.value( key, NULL );
	if( intc != NULL ) {
		intc->update( position, height, radius, course, speed );
	} else {
		intc = m_factory->createInterception( friendBplaId, enemyBplaId, position, height, radius, course,
                                              speed,time );
		intc->updateMap();
		m_interceptionList.insert( key, intc );
	}
}

void MapClient1::redrawAllBpla()
{
	foreach( MapFeature::FriendBpla* bpla, m_friendBplaList ) {
		bpla->updateMap();
	}

	//	foreach( MapFeature::EnemyBpla* bpla, m_enemyBplaList ) {
	//			bpla->updateMap();
	//	}
}
