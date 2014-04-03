#include "MapController.h"
#include <QPixmap>
#include <QLabel>
#include <QCursor>

#include "../Icons/IconsGenerator.h"

#include "../UAV/ZInterception.h"

MapController::MapController() :
	m_widget( new MapWidget( this ) ),
	m_map( new Map() )
{
	m_map->setMapManager( m_widget->getPwGis()->mapProvider()->mapManager() );
	m_map->setProfileManager( m_widget->getPwGis()->mapProvider()->profileManager() );

	QObject::connect( m_map, SIGNAL( modelMapReady( )), this, SLOT( onMapReady() ) );

	m_widget->getPwGis()->enableDebugger( false );
	connect(m_widget, SIGNAL( showBLAtree() ), this, SIGNAL( controllerShowBLAtree() ) );
	connect(m_widget, SIGNAL( showBPLAtree() ), this, SIGNAL( controllerShowBPLAtree() ) );
	connect(m_widget, SIGNAL( showNIIPP() ), this, SIGNAL( controllerShowNIIPP() ) );
}

MapController::~MapController()
{
}

void MapController::init( QMap<int, TabsProperty *> map_settings, IDBManager* db_bla,
	IDBManager* db_evil )
{
	m_map->init( map_settings, db_bla, db_evil, m_widget->getPwGis() );
}

void MapController::openMapFromAtlas()
{
	m_map->openAtlas();
}

void MapController::openMapFromLocalFile(/*const QString mapFile*/)
{
	QString filename = QFileDialog::getOpenFileName(
		m_widget,
		tr( "Открыть карту" ),
		QDir::currentPath(),
		tr( "Формат цифровых карт (*.chart *.sxf *.sit *.map *.gc *.gst);;Все файлы (*.*)" ) );
	QFile file( filename );
	if ( !file.exists() ) {
		return;
	}

	m_map->openMap( filename );
}

void MapController::onMapReady()
{
	get_panel_widget()->setMouseTracking( true );
	m_map->setLayerManager( m_widget->getPwGis()->mapProvider()->layerManager() );
	emit mapOpened();
}

void MapController::_slot_station_visible( bool state )
{
	m_map->setStationVisible( state );
}

PwGisWidget *MapController::get_pwwidget()
{
	return m_widget->getPwGis();
}

QWidget *MapController::get_widget()
{
	return m_widget->getWidget();
}

bool MapController::eventFilter( QObject *obj, QEvent *e )
{
	return true;
}

/// get map client by name
IMapClient *MapController::get_map_client( int id )
{
	return m_map->getMapClient(id);
}

/// get panel widget
QWidget *MapController::get_panel_widget()
{
	return m_widget->getPanelWidget();
}
