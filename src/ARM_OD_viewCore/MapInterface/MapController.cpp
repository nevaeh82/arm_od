#include "MapController.h"
#include <QPixmap>
#include <QLabel>
#include <QCursor>

#include "../Icons/IconsGenerator.h"

#include "../UAV/ZInterception.h"

MapController::MapController()
	: m_controllerWidget(new MapControllerWidget(this))
	, m_mapModel(new Map())
{
	m_mapModel->setMapManager(m_controllerWidget->getPwGis()->mapProvider()->mapManager());
	m_mapModel->setProfileManager(m_controllerWidget->getPwGis()->mapProvider()->profileManager());

	QObject::connect(m_mapModel, SIGNAL(modelMapReady()), this, SLOT(onMapReady()));

	m_controllerWidget->getPwGis()->enableDebugger(false);
	connect(m_controllerWidget, SIGNAL(showBLAtree()), this, SIGNAL(controllerShowBLAtree()));
	connect(m_controllerWidget, SIGNAL(showBPLAtree()), this, SIGNAL(controllerShowBPLAtree()));
	connect(m_controllerWidget, SIGNAL(showNIIPP()), this, SIGNAL(controllerShowNIIPP()));
}

MapController::~MapController()
{
}

void MapController::init(QMap<int, TabsProperty *> map_settings, IDBManager* db_bla, IDBManager* db_evil)
{
	m_mapModel->init(map_settings, db_bla, db_evil, m_controllerWidget->getPwGis());
}

void MapController::openMapFromAtlas()
{
	m_mapModel->openAtlas();
}

void MapController::openMapFromLocalFile(/*const QString mapFile*/)
{
	QString filename = QFileDialog::getOpenFileName(
		m_controllerWidget,
		tr("Open atlas"),
		QDir::currentPath(),
		tr("Atlas files (*.chart *.sxf *.sit *.map *.gc *.gst);;All files (*.*)") );
	if( !filename.isNull() )
	{
	}

	m_mapModel->openMap(filename);
}

void MapController::onMapReady()
{
	get_panel_widget()->setMouseTracking(true);
	m_mapModel->setLayerManager(m_controllerWidget->getPwGis()->mapProvider()->layerManager());
	emit mapOpened();
}

void MapController::_slot_station_visible(bool state)
{
	m_mapModel->setStationVisible(state);
}

PwGisWidget *MapController::get_pwwidget()
{
	return m_controllerWidget->getPwGis();
}

QWidget *MapController::get_widget()
{
	return m_controllerWidget->getWidget();
}

bool MapController::eventFilter(QObject *obj, QEvent *e)
{
	return true;
}

/// get map client by name
IMapClient *MapController::get_map_client(int id)
{
	return m_mapModel->getMapClient(id);
}

/// get panel widget
QWidget *MapController::get_panel_widget()
{
	return m_controllerWidget->getPanelWidget();
}
