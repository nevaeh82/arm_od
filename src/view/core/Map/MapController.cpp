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
	m_enemyModel = NULL;
	m_ourModel = NULL;

	m_mapModel = new Map(this);
	m_pelengEvilIds = 0;

	m_initJsTimer = new QTimer(this);
	m_initJsTimer->setInterval( MAP_INITJS_TIMEOUT );

	m_mapUpTime = QTime::currentTime();
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

	connect( m_mapModel, SIGNAL(modelMapReady()), this, SLOT(onMapReady()) );
	connect( m_view->getPwGis(), SIGNAL(mapClicked(double,double)), this, SLOT(onMapClicked(double,double)) );
    connect( m_view->getPwGis(), SIGNAL(mapMouseMove(double,double)), this, SLOT(onMapMouseMove(double,double)) );

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

	loadObjects();


	emit mapOpened();
}

void MapController::onMapMouseMove(double lon, double lat)
{
    IMapClient* client = getMapClient();
    if( NULL == client ) return;


    client->mapMoved(lon, lat);
}

void MapController::onMapClicked(double lon, double lat)
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	QPointF point( lon, lat );

	emit mapClicked( lon, lat );

    client->mapClicked(lon, lat);

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

void MapController::clearObjects()
{
	m_view->getPwGis()->executeScript( QString("map.getLayer(\"%1\").removeAllFeatures();").arg("vectors") );
	m_view->getPwGis()->executeScript( QString("map.getLayer(\"%1\").removeAllFeatures();").arg("markers") );
}

void MapController::loadObjects()
{
	QFile f(MAPOBJECTS_CACHE);
	f.open(QIODevice::ReadOnly);
	QString js = QString(f.readAll());
	f.close();

	&m_view->getPwGis()->executeScript("var GEOJSON_PARSER = new OpenLayers.Format.GeoJSON();");
	&m_view->getPwGis()->executeScript(QString("client.vectorLayer.addFeatures(GEOJSON_PARSER.read(%1))").arg(js)).toString();

	QFile fm(MAPMARKERS_CACHE);
	fm.open(QIODevice::ReadOnly);
	js = QString(fm.readAll());
	fm.close();

	&m_view->getPwGis()->executeScript(QString("client.markerLayer.addFeatures(GEOJSON_PARSER.read(%1))").arg(js)).toString();
}

void MapController::saveObjects()
{
	saveCache();
}

void MapController::mapOpenFinished()
{
	if( !m_view->getPwGis()->executeScript("SkyHobbitCheckReady()").toBool() ) {
		m_initJsTimer->singleShot(MAP_INITJS_TIMEOUT, this, SLOT(mapOpenFinished()));
	}
}

void MapController::onExtraBoardInfo(int val)
{
	IMapClient* client = getMapClient();
	client->setBoardExtraInfo(val);
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

    connect(m_mapModel, SIGNAL(onSquare(QString)), m_view, SLOT(onSetSquareVal(QString)));
    connect(this, SIGNAL(onMovingAimAlarm(QString)), m_view, SLOT(onSetMovingAlarm(QString)));
}

void MapController::saveCache()
{
	m_mapModel->saveCache();
}

void MapController::setEnemyModel(UavTreeModel *model)
{
	m_enemyModel = model;
}

void MapController::setOurModel(UavTreeModel *model)
{
	m_ourModel = model;
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

	m_mapUpTimeout = settings.value("Map/mapUpTimeout", 1000).toInt();

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

		if(!uavInfo.historical) {
			return;
		}

		client->addFriendBpla( uavInfo );
		return;
	}

	if( uavRole == ENEMY_UAV_ROLE ) {

		if(!uavInfo.historical) {
			return;
		}

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
	} else if(method == RPC_SLOT_SERVER_SEND_BLA_POINTS) {
		QDataStream inputDataStream(&data, QIODevice::ReadOnly);
		QVector<UAVPositionData> positionDataVector;
		inputDataStream >> positionDataVector;

		if (positionDataVector.size() < 1) {
			log_debug("Size uavpositiondata vector < 1");
			return;
		}

		//            QString dataToFile = QTime::currentTime().toString("hh:mm:ss:zzz") + " " + QString::number(positionDataVector.at(0).latitude) + " " + QString::number(positionDataVector.at(0).longitude) + " " + QString::number(positionDataVector.at(0).altitude) + "\n";
		//            m_logManager->writeToFile(dataToFile);

		/// Now we take first point, but we need to take more than 1 point
		UAVPositionData positionData = positionDataVector.at(0);
		positionData.name = QString::number(positionData.boardID);
		positionData.state = 1;

		UavInfo uav;
		uav.lat = positionDataVector.at(0).latitude;
		uav.lon = positionDataVector.at(0).longitude;
		uav.alt = positionDataVector.at(0).altitude;
		uav.yaw = positionDataVector.at(0).course;
		uav.name = QString::number(positionData.boardID);
		uav.id = positionData.boardID;

		//onUavInfoChanged(uav, OUR_UAV_ROLE);
		uav.source = positionDataVector.at(0).sourceType;
		client->addFriendBpla( uav );

		if(m_ourModel) {
			m_ourModel->onUavInfoChanged(uav, OUR_UAV_ROLE);
		}
	} else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS_1) {


		SolverProtocol::Packet pkt;
		pkt.ParseFromArray( data.data(), data.size() );

		if( (	pkt.has_datafromsolver() &&
				pkt.datafromsolver().has_solution_manual_altitude() &&
				pkt.datafromsolver().solution_manual_altitude().has_positionlines()) ||
				(pkt.has_datafromsolver() &&
				pkt.datafromsolver().has_solution_automatic_altitude() &&
				pkt.datafromsolver().solution_automatic_altitude().has_positionlines()) )
		{

			if(m_mapUpTime.msecsTo(QTime::currentTime()) < m_mapUpTimeout) {
				return;
			}
			m_mapUpTime = QTime::currentTime();

		}


		// HYPERBOLE
		if( pkt.has_datafromsolver() &&
				pkt.datafromsolver().has_solution_manual_altitude() &&
				pkt.datafromsolver().solution_manual_altitude().has_positionlines() ) {

			SolverProtocol::Packet_DataFromSolver_SolverSolution_PositionLines plMsg =
					pkt.datafromsolver().solution_manual_altitude().positionlines();
			QByteArray dataPL;
			dataPL.resize(plMsg.ByteSize());
			plMsg.SerializeToArray(dataPL.data(), dataPL.size());

			client->addHyperboles(dataPL, 1);
		} else if( pkt.has_datafromsolver() &&
				   pkt.datafromsolver().has_solution_automatic_altitude() &&
				   pkt.datafromsolver().solution_automatic_altitude().has_positionlines() ) {

			SolverProtocol::Packet_DataFromSolver_SolverSolution_PositionLines plMsg =
					pkt.datafromsolver().solution_automatic_altitude().positionlines();
			QByteArray dataPL;
			dataPL.resize(plMsg.ByteSize());
			plMsg.SerializeToArray(dataPL.data(), dataPL.size());

			client->addHyperboles(dataPL, 1);
		}

		// -----------------

		//SINGLEMARK

		if(pkt.has_datafromsolver() && pkt.datafromsolver().has_solution_manual_altitude() &&
				pkt.datafromsolver().solution_manual_altitude().has_singlemarks()) {

			SolverProtocol::Packet_DataFromSolver_SolverSolution_SingleMarks sMsg =  pkt.datafromsolver().solution_manual_altitude().singlemarks();

			QByteArray msg;
			msg.resize(sMsg.ByteSize());
			sMsg.SerializeToArray(msg.data(), msg.size());
			//Draw it from trajectories temrorally too. - next if
			//here draw only ellipse
			client->addSingleMark(msg);
		}

		if(pkt.has_datafromsolver() && pkt.datafromsolver().has_solution_automatic_altitude() &&
				pkt.datafromsolver().solution_automatic_altitude().has_singlemarks()) {

			SolverProtocol::Packet_DataFromSolver_SolverSolution_SingleMarks sMsg =
					pkt.datafromsolver().solution_automatic_altitude().singlemarks();

			QByteArray msg;
			msg.resize(sMsg.ByteSize());
			sMsg.SerializeToArray(msg.data(), msg.size());
			//Draw it from trajectories temrorally too. - next if
			//here draw only ellipse
			client->addSingleMark(msg);
		}

		//Error ellipse

		if( isSolverMessageHasTrajectoryManual(pkt) ) {
			//Trajectory from db. No kk here
			QByteArray msg;
			int size = pkt.datafromsolver().solution_manual_altitude().trajectory_size();
			for(int i = 0; i<size; i++) {
				SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory sol =
						pkt.datafromsolver().solution_manual_altitude().trajectory(i);
				msg.resize(sol.ByteSize());
				sol.SerializeToArray(msg.data(), msg.size());
				client->addTrajectoryKK( msg, 101 );

				if(m_enemyModel) {
					UavInfo uav;
					uav.lat = sol.motionestimate(sol.motionestimate_size()-1).coordinates().lat();
					uav.lon = sol.motionestimate(sol.motionestimate_size()-1).coordinates().lon();
					uav.alt = sol.motionestimate(sol.motionestimate_size()-1).coordinates().alt();
					uav.yaw = sol.motionestimate(sol.motionestimate_size()-1).relativebearing();
					uav.name = QString::fromStdString(sol.targetid());

					m_enemyModel->onUavInfoChanged(uav, ENEMY_UAV_ROLE);
				}
			}
		}

		if( isSolverMessageHasTrajectoryAuto(pkt) ) {
			//Trajectory from db. No kk here
			QByteArray msg;
			int size = pkt.datafromsolver().solution_automatic_altitude().trajectory_size();
            bool movingAim = false;

			for(int i = 0; i<size; i++) {
				SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory sol =
						pkt.datafromsolver().solution_automatic_altitude().trajectory(i);

				msg.resize(sol.ByteSize());
				sol.SerializeToArray(msg.data(), msg.size());
				client->addTrajectoryKK( msg, 100 );

                if( SolverProtocol::StateOfMotion::MOVING ==
                        sol.motionestimate(sol.motionestimate_size()-1).state() ) {
                    emit onMovingAimAlarm(tr("Moving aim on %1 mHz").arg(sol.central_frequency()));
                    movingAim = true;
                }

				if(m_enemyModel) {
					UavInfo uav;
					uav.lat = sol.motionestimate(sol.motionestimate_size()-1).coordinates().lat();
					uav.lon = sol.motionestimate(sol.motionestimate_size()-1).coordinates().lon();
					uav.alt = sol.motionestimate(sol.motionestimate_size()-1).coordinates().alt();
					uav.yaw = sol.motionestimate(sol.motionestimate_size()-1).relativebearing();
					uav.name = QString::fromStdString(sol.targetid());

					m_enemyModel->onUavInfoChanged(uav, ENEMY_UAV_ROLE);
				}
			}

            if(!movingAim) {
                emit onMovingAimAlarm(QString(""));
            }
		}

		//Draw stations and area from settings Solver responce
		if( isSolverMessageSolverResponse( pkt ) ) {
			SolverProtocol::Packet_DataFromSolver_SolverResponse response = pkt.datafromsolver().solverresponse();

			if( response.has_detectors() ) {

				QByteArray dataToSend;
				dataToSend.resize( response.detectors().ByteSize() );
				response.detectors().SerializeToArray(dataToSend.data(), dataToSend.size());

				client->addStation( dataToSend );
			}

			if(response.has_areaofresponsibility()) {
				QPointF point1(response.areaofresponsibility().mincoordinates().lon(),
							   response.areaofresponsibility().mincoordinates().lat());
				QPointF point2(response.areaofresponsibility().maxcoordinates().lon(),
							   response.areaofresponsibility().maxcoordinates().lat());
				client->addWorkArea( point1, point2 );
			}
		}

		//get messages
		if( isSolverMessageSolverMessage(pkt) ) {
			QString msg = QString::fromStdString(pkt.datafromsolver().message().message());

			if(msg.contains(QString::fromLocal8Bit("очищен"), Qt::CaseInsensitive)) {
				client->clearSolver1();
			}
		}
	}
}
