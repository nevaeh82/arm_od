#include "MapController.h"
#include <QPixmap>
#include <QLabel>
#include <QCursor>

#include "Logger.h"

#include "../Icons/IconsGenerator.h"

#include "../UAV/ZInterception.h"

MapController::MapController(QObject *parent):
	QObject(parent)
{
	m_view = NULL;

	m_mapModel = new Map(this);
	m_pelengEvilIds = 0;
	m_rdsEvilIds = 50;
}

MapController::~MapController()
{
	log_debug("~MapController()");
}

void MapController::init(QMap<int, Station*> map_settings)
{
	m_mapModel->setMapManager(m_view->getPwGis()->mapProvider()->mapManager());
	m_mapModel->setProfileManager(m_view->getPwGis()->mapProvider()->profileManager());

	connect(m_mapModel, SIGNAL(modelMapReady()), this, SLOT(onMapReady()));

	m_view->getPwGis()->enableDebugger(false);

	m_mapModel->init(map_settings, m_view->getPwGis());
}

void MapController::openMapFromAtlas()
{
	m_mapModel->openAtlas();
}

void MapController::openMapFromLocalFile(/*const QString mapFile*/)
{
	QString filename = QFileDialog::getOpenFileName(
		m_view,
		tr("Open atlas"),
		QDir::currentPath(),
		tr("Atlas files (*.chart *.sxf *.sit *.map *.gc *.gst);;All files (*.*)") );
	if( !filename.isNull() )
	{
		m_mapModel->openMap(filename);
	}
}

void MapController::onMapReady()
{
	//get_panel_widget()->setMouseTracking(true);
	m_mapModel->setLayerManager(m_view->getPwGis()->mapProvider()->layerManager());
	emit mapOpened();
}

void MapController::_slot_station_visible(bool state)
{
	m_mapModel->setStationVisible(state);
}


/// get map client by name
IMapClient *MapController::getMapClient(int id)
{
	return m_mapModel->getMapClient(id);
}

void MapController::appendView(MapWidget *view)
{
	m_view = view;
}

void MapController::closeMap()
{
	m_mapModel->closeMap();
}

void MapController::closeAtlas()
{
	m_mapModel->closeAtlas();
}

/// TODO : Refactor this peace of shit
void MapController::onMethodCalled(const QString& method, const QVariant& argument)
{
	int mapClientId = 1; // I do not know, why.. Don't ask me.
	QByteArray data = argument.toByteArray();

	if (method == RPC_SLOT_SERVER_SEND_BLA_POINTS) {

		QDataStream inputDataStream(&data, QIODevice::ReadOnly);
		QVector<UAVPositionData> positionDataVector;
		inputDataStream >> positionDataVector;

		if (positionDataVector.size() < 1) {
			log_debug("Size uavpositiondata vector < 1");
			return;
		}

		/// Now we take first point, but we need to take more than 1 point
		UAVPositionData positionData = positionDataVector.at(0);

		int id = positionData.boardID; /// need quint16
		QPointF point;
		point.setX(positionData.latitude);
		point.setY(positionData.longitude);
		double alt = positionData.altitude;
		double speed = positionData.speed;
		double course = positionData.course;
		int state = positionData.state;

		QByteArray ddd;
		QDataStream ds(&ddd, QIODevice::WriteOnly);
		ds << point;
		ds << alt;
		ds << speed;
		ds << course;
		ds << state;

		IMapClient* client = getMapClient(mapClientId);

		if (NULL == client) {
			return;
		}

		client->addFriendBpla(id, ddd);
	} else if (method == RPC_SLOT_SERVER_SEND_AIS_DATA) {

		QDataStream ds(&data, QIODevice::ReadOnly);
		QMap<int, QVector<QString> > map;
		ds >> map;

		IMapClient* client = getMapClient(mapClientId);

		if (NULL == client) {
			return;
		}

		client->addAis(map);
	} else if (method == RPC_SLOT_SERVER_SEND_NIIPP_DATA) {

		QDataStream ds(&data, QIODevice::ReadOnly);

		QDateTime dt;
		QTime time;
		int mode;
		QPointF point;
		QString NS;
		QString EW;
		int alt;
		int zone;
		int course;
		int angle;

		int id;

		ds >> id;

		ds >> dt;
		ds >> time;
		ds >> mode;
		ds >> point;
		ds >> NS;
		ds >> EW;
		ds >> alt;
		ds >> zone;
		ds >> course;
		ds >> angle;

		QByteArray ba;
		QDataStream ds1(&ba, QIODevice::WriteOnly);

		QPointF latlon;
		if (id == 100) {
			latlon.setX(42.511183);
			latlon.setY(41.6905);
		}
		if (id == 101) {
			latlon.setX(42.634183);
			latlon.setY(41.912167);
		}

		QString name = QString::number(id) + " - niipp";
		double wid = 25;
		ds1 << name;
		ds1 << latlon;
		ds1 << wid;


		if(mode == 01) {
			IMapClient* client = getMapClient(mapClientId);

			if (NULL == client) {
				return;
			}

			client->updateNiippPowerCicle(id, m_zone[zone], ba);
		}

		if(mode == 10) {
			IMapClient* client = getMapClient(mapClientId);

			if (NULL == client) {
				return;
			}

			client->updateNiippPowerSector(id, m_zoneDir[zone], course, NULL);
		}

		/// TODO: recheck following. WTF?!
//		QByteArray ba1;
//		QDataStream ds2(&ba1, QIODevice::WriteOnly);
//		ds2 << mode;
//		m_tabManager->send_data_niipp_control(id, ba1);
	} else if (method == RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION) {

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

		IMapClient* client = getMapClient(mapClientId);

		if (NULL == client) {
			return;
		}

		client->updatePeleng(msg.sourceId, id_post, msg.postLatitude, msg.postLongitude, msg.direction);
	} else if (method == RPC_SLOT_SERVER_SEND_ATLANT_POSITION) {

		QDataStream ds(&data, QIODevice::ReadWrite);
		A_Pos_Ans_msg msg;
		ds >> msg.requestId;
		ds >> msg.sourceId;
		ds >> msg.dateTime;
		ds >> msg.latitude;
		ds >> msg.longitude;
		ds >> msg.quality;

		if(!m_mapPelengEvilIds.contains(msg.sourceId)) {
			m_mapPelengEvilIds.insert(msg.sourceId, ++m_pelengEvilIds);
			if(m_pelengEvilIds > 49) {
				m_pelengEvilIds = 0;
				m_mapPelengEvilIds.clear();
			}
		}

		QPointF point(msg.longitude, msg.latitude);

		QByteArray ba1;
		QDataStream ds1(&ba1, QIODevice::WriteOnly);
		ds1 << point;

		log_debug(QString("ID = %1 %2 %3").arg(m_mapPelengEvilIds.size()).arg(m_pelengEvilIds).arg(m_mapPelengEvilIds.value(msg.sourceId)));

		IMapClient* client = getMapClient(mapClientId);

		if (NULL == client) {
			return;
		}

		client->addEnemyBpla(m_mapPelengEvilIds.value(msg.sourceId), ba1);
	} else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS) {
		sendEnemyUavPoints(data, mapClientId);
	} else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO) {
		sendEnemyUavPoints(data, mapClientId);
	}
}

void MapController::sendEnemyUavPoints(const QByteArray& data, const int& mapClientId)
{
	QByteArray inputData = data;
	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);
	UAVPositionDataEnemy uavEnemy;
	inputDataStream >> uavEnemy;

	QByteArray oldDataFormat;
	QDataStream ds(&oldDataFormat, QIODevice::WriteOnly);
	ds << 10 << uavEnemy.state << uavEnemy.pointStdDev << uavEnemy.track << uavEnemy.speed
	   << uavEnemy.altitude << uavEnemy.course;

	if(m_rdsEvilIds > 99){
		m_rdsEvilIds = 50;
	}

	IMapClient* client =  getMapClient(mapClientId);

	if (NULL == client) {
		return;
	}

	client->addEnemyBpla(m_rdsEvilIds, oldDataFormat);
}
