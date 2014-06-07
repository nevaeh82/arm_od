#include "MapController.h"
#include <QPixmap>
#include <QLabel>
#include <QCursor>

#include <Logger.h>

#include <MapManagerEvents.h>

#include "Icons/IconsGenerator.h"
#include "UAV/ZInterception.h"
#include "Tabs/DbBla/Defines.h"

MapController::MapController(QObject *parent):
	QObject(parent)
{
	m_view = NULL;
	m_mapModel = new Map(this);
	m_pelengEvilIds = 0;
}

MapController::~MapController()
{
	log_debug("~MapController()");
}

void MapController::init(QMap<int, Station*> stations)
{
	m_mapModel->setMapManager( m_view->getPwGis()->mapProvider()->mapManager() );
	m_mapModel->setProfileManager( m_view->getPwGis()->mapProvider()->profileManager() );
	m_mapModel->init( stations, m_view->getPwGis() );

	m_view->getPwGis()->enableDebugger(false);

	connect( m_mapModel, SIGNAL(modelMapReady()), this, SLOT(onMapReady()) );
	connect( m_view->getPwGis(), SIGNAL(mapClicked(double,double)), this, SLOT(onMapClicked(double,double)) );
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
	}
	else {
		emit cancelMapOpen();
	}
}

void MapController::onMapReady()
{
	//get_panel_widget()->setMouseTracking(true);
	m_mapModel->setLayerManager(m_view->getPwGis()->mapProvider()->layerManager());
	emit mapOpened();
}

void MapController::onMapClicked(double lon, double lat)
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	QPointF point( lon, lat );

	client->addNiippPoint( point );

	if( m_niippControllers.contains( 100 ) ) {
		m_niippControllers.value( 100 )->setPoint( point );
	}

	if( m_niippControllers.contains( 101 ) ) {
		m_niippControllers.value( 101 )->setPoint( point );
	}
}

void MapController::setStationVisible(bool state)
{
	m_mapModel->setStationVisible(state);
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

void MapController::onUavRemoved(const Uav& uav, const QString&)
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	client->removeBpla( uav );
}

void MapController::onUavInfoChanged(const UavInfo& uavInfo, const QString& uavRole)
{
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	if( uavRole == OUR_UAV_ROLE ) {
		client->addFriendBpla( uavInfo );
		return;
	}

	if( uavRole == ENEMY_UAV_ROLE ) {
		client->addEnemyBpla( uavInfo );
		return;
	}
}

/// \todo Refactor this peace of shit
void MapController::onMethodCalled(const QString& method, const QVariant& argument)
{
	double f;
	int c;
	IMapClient* client = getMapClient();
	if( NULL == client ) return;

	QByteArray data = argument.toByteArray();

	if( method == RPC_SLOT_SERVER_SEND_AIS_DATA ) {
		QDataStream ds(&data, QIODevice::ReadOnly);
		QMap<int, QVector<QString> > map;
		ds >> map;

		client->addAis(map);
	} else if( method == RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION ) {

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
		client->addHyperbole(data, QTime::currentTime());
	}
}
