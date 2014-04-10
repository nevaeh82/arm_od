#include "Map.h"

Map::Map()
{
	m_layersCounter = 0;
}

Map::~Map()
{
	m_mapManager->closeMap();
}

void Map::init( QMap<int, Station*> stations, PwGisWidget* pwwidget )
{
	m_stations = stations;

	QMap<int, Station*>::iterator it;
	for (it = m_stations.begin(); it != m_stations.end(); ++it)
	{
		m_clients[(it.value())->id] = new MapClient1( pwwidget, it.value() );
	}
}

void Map::setMapManager( Pw::Gis::IMapManager* mapManager )
{
	m_mapManager = mapManager;
	m_mapManager->setProfile( "Zav" );

	connect( &m_mapManager->events(), SIGNAL( mapReady() ), SLOT( onMapReady() ) );
}

void Map::setProfileManager( Pw::Gis::IProfileManager* profileManager )
{
	m_profileManager = profileManager;
	m_profileManager->registerProfile( "Zav", ":/images/" );
}

void Map::setLayerManager( Pw::Gis::ILayerManager* layerManager )
{
	m_layerManager = layerManager;
}

void Map::setStationVisible( bool state )
{
	m_clients.value(1)->showLayer( 0, state );
}

bool Map::openAtlas()
{
	return m_mapManager->openAtlas();
}

bool Map::openMap( QString fileName )
{
	return m_mapManager->openMap( fileName, 100 );
}

void Map::onMapReady()
{
	QMap<int, IMapClient *>::iterator it;

	for( it = m_clients.begin(); it != m_clients.end(); ++it ) {
		if ( !it.value() ) continue;

		IMapClient *client = it.value();

		client->init();
		client->setPoint();
	}

	emit modelMapReady();
}

IMapClient* Map::getMapClient( int id )
{
	return m_clients[id];
}
