#include "MapController.h"
#include <QPixmap>
#include <QLabel>
#include <QCursor>

#include <Logger/Logger.h>

#include <MapManagerEvents.h>

#include "Icons/IconsGenerator.h"
#include "UAV/ZInterception.h"
#include "Tabs/DbBla/Defines.h"

#include "SolverExchange.h"
//#include "SolverPacket1.pb.h"

MapController::MapController(QObject *parent):
	QObject(parent)
{
	m_view = NULL;
	m_mapModel = new Map(this);
	m_pelengEvilIds = 0;

	m_initJsTimer = new QTimer(this);
	m_initJsTimer->setInterval( MAP_INITJS_TIMEOUT );
}

MapController::~MapController()
{
	log_debug("~MapController()");
}

void MapController::setViewport(QString viewport)
{
	m_viewport = QRectF();

	QStringList parts = viewport.split(" ");
	bool toDouble;


	if (parts.size() < 4) return;

	double left = parts[0].toDouble( &toDouble );
	if (!toDouble) return;

	double top = parts[1].toDouble( &toDouble );
	if (!toDouble) return;

	double right = parts[2].toDouble( &toDouble );
	if (!toDouble) return;

	double bottom = parts[3].toDouble( &toDouble );
	if (!toDouble) return;

	top = qMax( top, bottom );
	left = qMin( left, right );
	right = qMax( left, right );
	bottom = qMin( top, bottom );

	m_viewport = QRectF( QPointF( left, top ), QPointF( right, bottom ) );
}

void MapController::init(QMap<int, Station*> stations)
{
	m_mapModel->setMapManager( m_view->getPwGis()->mapProvider()->mapManager() );
	m_mapModel->setProfileManager( m_view->getPwGis()->mapProvider()->profileManager() );
	m_mapModel->init( stations, m_view );

	m_view->getPwGis()->enableDebugger(false);

	connect( m_mapModel, SIGNAL(modelMapReady()), this, SLOT(onMapReady()) );
	connect( m_view->getPwGis(), SIGNAL(mapClicked(double,double)), this, SLOT(onMapClicked(double,double)) );

	connect(&m_view->getPwGis()->mapProvider()->mapManager()->events(),
			SIGNAL(mapReady()), this, SLOT(mapOpenFinished()));
}

void MapController::openMapFromAtlas()
{
	m_mapModel->openAtlas();
}

void MapController::openMapFromLocalFile()
{
	QString filename = QFileDialog::getOpenFileName(
		m_view,
		tr( "Open map" ),
		QDir::currentPath(),
		tr( "Map files (*.chart *.sxf *.sit *.map *.gc *.gst)" ) );
	if ( !filename.isNull() ) {
		m_mapModel->openMap(filename);
	} else {
		emit mapOpenCanceled();
	}
}

void MapController::onMapReady()
{
	//get_panel_widget()->setMouseTracking(true);
	m_mapModel->setLayerManager(m_view->getPwGis()->mapProvider()->layerManager());

	if ( !m_viewport.isNull() ) {
		m_view->getPwGis()->mapProvider()->mapBounds()->zoomMapTo(
					m_viewport.left(), m_viewport.top(),
					m_viewport.right(), m_viewport.bottom() );
	}

	emit mapOpened();
}

void MapController::onMapClicked(double lon, double lat)
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	QPointF point( lon, lat );

//	client->addNiippPoint( point );

//	if( m_niippControllers.contains( 100 ) ) {
//		m_niippControllers.value( 100 )->setPoint( point );
//	}

//	if( m_niippControllers.contains( 101 ) ) {
//		m_niippControllers.value( 101 )->setPoint( point );
//	}
}

void MapController::setStationVisible(bool state)
{
	m_mapModel->setStationVisible(state);
}

void MapController::mapOpenFinished()
{
	if( !m_view->getPwGis()->executeScript("SkyHobbitCheckReady()").toBool() ) {
		m_initJsTimer->singleShot(MAP_INITJS_TIMEOUT, this, SLOT(mapOpenFinished()));
	}
}

IMapClient *MapController::getMapClient(int id)
{
    return m_mapModel->getMapClient(id);
}

void MapController::setNiippController(INiippController* controller)
{
	m_niippControllers.insert( controller->getId(), controller );
}

void MapController::updateNiippPowerZone(const Niipp& niipp)
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	client->updateNiippPowerZone( niipp );
}

void MapController::removeNiippPoint()
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	client->removeNiippPoint();
}

void MapController::appendView(MapWidget *view)
{
	Pw::Gis::MapManagerEvents* events;

	if( m_view != NULL ) {
		events = &m_view->getPwGis()->mapProvider()->mapManager()->events();
		disconnect( events, SIGNAL(atlasReady()), this, SIGNAL(atlasOpened()) );
	}

	m_view = view;

	if( m_view != NULL ) {
		events = &m_view->getPwGis()->mapProvider()->mapManager()->events();
		connect( events, SIGNAL(atlasReady()), SIGNAL(atlasOpened()) );
	}
}

void MapController::closeMap()
{
	m_mapModel->closeMap();
}

void MapController::closeAtlas()
{
	m_mapModel->closeAtlas();
}

IMapClient* MapController::getMapClient()
{
	int mapClientId = 1; // I do not know, why.. Don't ask me.
	return getMapClient( mapClientId );
}

void MapController::loadMapSettings()
{
	// load map settings form client.ini
	QString path = QApplication::applicationDirPath() + "/Client.ini";
	QSettings settings(path, QSettings::IniFormat);

	QString mapFile = settings.value("Map/file").toString();
	QString viewport = settings.value("Map/viewport").toString();

	// open default map
	if (!mapFile.isEmpty()) {
		if ( mapFile.indexOf("internet/") == 0 ) {
			m_view->getPwGis()->mapProvider()->mapManager()->openAtlasMap( 0, mapFile );
		} else {
			m_view->getPwGis()->mapProvider()->mapManager()->openMap( mapFile );
		}
	}

	setViewport(viewport);
}

void MapController::onUavRemoved(const Uav& uav, const QString&)
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	client->removeBpla( uav );
}

void MapController::onUavInfoChanged(const UavInfo& uavInfo, const QString& uavRole,
									 const QVector<QPointF>& tail,
									 const QVector<QPointF>& tailStdDev)
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	if( uavRole == OUR_UAV_ROLE ) {
		client->addFriendBpla( uavInfo );
		return;
	}

	if( uavRole == ENEMY_UAV_ROLE ) {
		client->addEnemyBpla( uavInfo, tail, tailStdDev );
		return;
	}
}

/// \todo Refactor this peace of shit
void MapController::onMethodCalled(const QString& method, const QVariant& argument)
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	QByteArray data = argument.toByteArray();

	if( method == RPC_SLOT_SERVER_SEND_AIS_DATA ) {
		QDataStream ds(&data, QIODevice::ReadOnly);
		QMap<int, QVector<QString> > map;
		ds >> map;

		client->addAis(map);
	} else if( method == RPC_SLOT_SERVER_SEND_ADSB_DATA ) {
		client->addAdsb( data );
	}
	else if( method == RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION ) {

		QDataStream ds(&data, QIODevice::ReadWrite);
		A_Dir_Ans_msg msg;
		ds >> msg.requestId;
		ds >> msg.sourceId;
		ds >> msg.dateTime;
		ds >> msg.post;
		ds >> msg.postLatitude;
		ds >> msg.postLongitude;
		ds >> msg.postHeight;
		ds >> msg.frequency;
		ds >> msg.widht;
		ds >> msg.direction;
		ds >> msg.angle;
		ds >> msg.level;
		ds >> msg.quality;
		ds >> msg.motionType;
		ds >> msg.motionConfidence;

		int id_post = msg.post.right(1).toInt();

		client->updatePeleng(msg.sourceId, id_post, msg.postLatitude, msg.postLongitude, msg.direction);
	} else if( method == RPC_SLOT_SERVER_SEND_ATLANT_POSITION ) {

		QDataStream ds(&data, QIODevice::ReadWrite);
		A_Pos_Ans_msg msg;
		ds >> msg.requestId;
		ds >> msg.sourceId;
		ds >> msg.dateTime;
		ds >> msg.latitude;
		ds >> msg.longitude;
		ds >> msg.quality;

		if( !m_mapPelengEvilIds.contains( msg.sourceId ) ) {
			m_mapPelengEvilIds.insert( msg.sourceId, ++m_pelengEvilIds );
			if( m_pelengEvilIds > 49 ) {
				m_pelengEvilIds = 0;
				m_mapPelengEvilIds.clear();
			}
		}

		UavInfo uav;
		uav.uavId = m_mapPelengEvilIds.value( msg.sourceId );
		uav.lon = msg.longitude;
		uav.lat = msg.latitude;

		client->addEnemyBpla( uav );
	}
	else if( method == RPC_SLOT_SERVER_SEND_HYPERBOLA ) {
		client->addHyperboles(data);
	}
    else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS_1) {
        SolverProtocol::Packet pkt;
        pkt.ParseFromArray( data.data(), data.size() );

        if( pkt.has_datafromsolver() &&
            pkt.datafromsolver().has_solution_manual_altitude() &&
            pkt.datafromsolver().solution_manual_altitude().has_positionlines() ) {

            client->addHyperboles(data, 1);
        }

        if(pkt.has_datafromsolver() && pkt.datafromsolver().has_solution_manual_altitude() &&
           pkt.datafromsolver().solution_manual_altitude().has_singlemarks()) {

            SolverProtocol::Packet_DataFromSolver_SolverSolution_SingleMarks sMsg =              pkt.datafromsolver().solution_manual_altitude().singlemarks();

            QByteArray msg;
            msg.resize(sMsg.ByteSize());
            sMsg.SerializeToArray(msg.data(), msg.size());
            //Draw it from trajectories temrorally too. - next if
            //here draw only ellipse
            client->addSingleMark(msg);
        }

        if( isSolverMessageHasTrajectoryManual(pkt) ) {

            for(int i = 0; i<pkt.datafromsolver().solution_manual_altitude().trajectory_size(); i++) {
                SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory sMsg =
                        pkt.datafromsolver().solution_manual_altitude().trajectory(i);

                QByteArray msg;
                msg.resize(sMsg.ByteSize());
                sMsg.SerializeToArray(msg.data(), msg.size());

                client->addTrajectoryKK( msg );
            }
        }

        //Draw stations and area from settings Solver responce
        if( isSolverMessageSolverResponse( pkt ) ) {
            SolverProtocol::Packet_DataFromSolver_SolverResponse response = pkt.datafromsolver().solverresponse();

            if( response.has_detectors() ) {
                for(int i = 0; i<response.detectors().detector_size(); i++ ) {
                    QString name = QString::fromStdString(  response.detectors().detector(i).detector_name() );
                    client->addStation( name,
                                        QPointF(response.detectors().detector(i).coords().lon(),
                                                response.detectors().detector(i).coords().lat())
                                        );
                }
            }

            if(response.has_areaofresponsibility()) {
                QPointF point1(response.areaofresponsibility().mincoordinates().lon(),
                               response.areaofresponsibility().mincoordinates().lat());
                QPointF point2(response.areaofresponsibility().maxcoordinates().lon(),
                               response.areaofresponsibility().maxcoordinates().lat());
                client->addWorkArea( point1, point2 );
            }
        }
    }
}
