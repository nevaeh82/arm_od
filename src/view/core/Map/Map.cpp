#include "Map.h"


Map::Map(QObject *parent ) :
	QObject(parent)
{
	m_layersCounter = 0;
	m_mapManager = 0;
	m_profileManager = 0;
	m_layerManager = 0;
}

Map::~Map()
{
	m_mapClients.clear();

	m_mapManager->closeMap();
	m_mapManager->closeAtlas();
}

void Map::init(QMap<int, Station*> map_settings, MapWidget* pwwidget)
{
	m_mapSettings = map_settings;

	QMap<int, Station*>::iterator it;
	for (it = m_mapSettings.begin(); it != m_mapSettings.end(); ++it)
	{
		MapClient1* client = new MapClient1(pwwidget, it.value(), this);
		m_mapClients[(it.value())->id] = client;

        m_firstClient = client;

		connect( this, SIGNAL(modelMapReady()), client, SLOT(init()) );
	}
}

void Map::setMapManager(Pw::Gis::IMapManager* mapManager)
{
	m_mapManager = mapManager;
	m_mapManager->setProfile("Zav");

	connect(&m_mapManager->events(), SIGNAL(mapReady()), this, SLOT(onMapReady()));
}

void Map::setProfileManager(Pw::Gis::IProfileManager* profileManager)
{
	m_profileManager = profileManager;
	m_profileManager->registerProfile("Zav", ":/images/");
}

void Map::setLayerManager(Pw::Gis::ILayerManager* layerManager)
{
	m_layerManager = layerManager;
}

void Map::setStationVisible(bool state)
{
	m_mapClients.value(1)->showLayer(0, state);
}

bool Map::openAtlas()
{
	return m_mapManager->openAtlas();
}

bool Map::openMap(QString fileName)
{
	return m_mapManager->openMap(fileName, 100);
}

void Map::closeAtlas()
{
	m_mapManager->closeAtlas();
}

void Map::closeMap()
{
	m_mapManager->closeMap();
}

void Map::onMapReady()
{
	QMap<int, IMapClient *>::iterator it;

	emit modelMapReady();

	for (it = m_mapClients.begin(); it != m_mapClients.end(); ++it) {
		if(!it.value()) continue;

		it.value()->setPoint();
	}
}

IMapClient* Map::getMapClient(int id)
{
	return m_mapClients[id];
}

IMapClient* Map::getFirstMapClient()
{
    return m_firstClient;
}
