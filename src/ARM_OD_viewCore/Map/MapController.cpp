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
