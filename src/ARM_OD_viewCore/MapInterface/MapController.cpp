#include "MapController.h"
#include <QPixmap>
#include <QLabel>
#include <QCursor>

#include "../Icons/IconsGenerator.h"

#include "../UAV/ZInterception.h"

MapController::MapController(QObject *parent):
	QObject(parent), m_mapModel(new Map())
{
	m_view = NULL;
}

MapController::~MapController()
{
}

void MapController::init(QMap<int, Station*> map_settings, IDBManager* db_bla, IDBManager* db_evil)
{
	m_mapModel->setMapManager(m_view->getPwGis()->mapProvider()->mapManager());
	m_mapModel->setProfileManager(m_view->getPwGis()->mapProvider()->profileManager());

	connect(m_mapModel, SIGNAL(modelMapReady()), this, SLOT(onMapReady()));

	m_view->getPwGis()->enableDebugger(false);
	connect(m_view, SIGNAL(showBLAtree()), this, SIGNAL(controllerShowBLAtree()));
	connect(m_view, SIGNAL(showBPLAtree()), this, SIGNAL(controllerShowBPLAtree()));
	connect(m_view, SIGNAL(showNIIPP()), this, SIGNAL(controllerShowNIIPP()));

	m_mapModel->init(map_settings, db_bla, db_evil, m_view->getPwGis());
}

void MapController::openMapFromAtlas()
{
	m_mapModel->openAtlas();
}

void MapController::openMapFromLocalFile(/*const QString mapFile*/)
{
	QString filename = QFileDialog::getOpenFileName(
		m_view,
		tr("Открыть карту"),
		QDir::currentPath(),
		tr("Формат цифровых карт (*.chart *.sxf *.sit *.map *.gc *.gst);;Все файлы (*.*)") );
	if( !filename.isNull() )
	{
	}

	m_mapModel->openMap(filename);
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

PwGisWidget *MapController::get_pwwidget()
{
	return m_view->getPwGis();
}

QWidget *MapController::get_widget()
{
	return m_view->getWidget();
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

void MapController::appendView(MapWidget *view)
{
	m_view = view;
}

///// get panel widget
//QWidget *MapController::get_panel_widget()
//{
//	return m_view->getPanelWidget();
//}
