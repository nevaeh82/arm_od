#include "MapController.h"
#include <QPixmap>
#include <QLabel>
#include <QCursor>

#include "Icons/IconsGenerator.h"

#include "UAV/ZInterception.h"

MapController::MapController() :
	m_view( NULL ),
	m_map( new Map() )
{
	QObject::connect( m_map, SIGNAL( modelMapReady( )), this, SLOT( onMapReady() ) );
}

MapController::~MapController()
{
}

void MapController::init(QMap<int, Station*> stations)
{
	if( m_view == NULL ) return;

	m_map->init( stations, m_view->getPwGis() );
}

void MapController::openMapFromAtlas()
{
	m_map->openAtlas();
}

void MapController::openMapFromLocalFile(/*const QString mapFile*/)
{
	if( m_view == NULL ) return;

	QString filename = QFileDialog::getOpenFileName(
		m_view,
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
	if( m_view == NULL ) return;

	getPanelWidget()->setMouseTracking( true );
	m_map->setLayerManager( m_view->getPwGis()->mapProvider()->layerManager() );
	emit mapOpened();
}

void MapController::setStationVisibility( bool state )
{
	m_map->setStationVisible( state );
}

PwGisWidget* MapController::getPwWidget()
{
	if( m_view == NULL ) return NULL;

	return m_view->getPwGis();
}

QWidget* MapController::getWidget()
{
	if( m_view == NULL ) return NULL;

	return m_view->getWidget();
}

/// get map client by name
IMapClient* MapController::getMapClient( int id )
{
	return m_map->getMapClient(id);
}

/// get panel widget
QWidget* MapController::getPanelWidget()
{
	if( m_view == NULL ) return NULL;

	return m_view->getPanelWidget();
}

void MapController::appendView(MapWidget *view)
{
	m_view = view;

	m_map->setMapManager( m_view->getPwGis()->mapProvider()->mapManager() );
	m_map->setProfileManager( m_view->getPwGis()->mapProvider()->profileManager() );

	m_view->getPwGis()->enableDebugger( false );
	connect(m_view, SIGNAL( showBLAtree() ), this, SIGNAL( showFriendBplaTreeCliecked() ) );
	connect(m_view, SIGNAL( showBPLAtree() ), this, SIGNAL( showEnemyBplaTreeClicked() ) );
	connect(m_view, SIGNAL( showNIIPP() ), this, SIGNAL( showNiippClicked() ) );
}
