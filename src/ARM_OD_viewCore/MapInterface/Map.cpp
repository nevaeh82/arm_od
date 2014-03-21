#include "Map.h"

Map::Map()
{
	m_layersCounter = 0;
}

Map::~Map()
{
	m_mapManager->closeMap();
}

void Map::init(QMap<int, TabsProperty *> map_settings, IDBManager* db_bla, IDBManager* db_evil, PwGisWidget* pwwidget)
{
	m_dbBla = db_bla;
	m_dbEvil = db_evil;
	m_mapSettings = map_settings;

	QMap<int, TabsProperty *>::iterator it;
	for (it = m_mapSettings.begin(); it != m_mapSettings.end(); ++it)
	{
		m_mapClients[(it.value())->get_id()] = new MapClient1(pwwidget, it.value(), m_dbBla, m_dbEvil);
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
	m_mapClients.value(1)->show_layer(0, state);
}

bool Map::openAtlas()
{
	return m_mapManager->openAtlas();
}

bool Map::openMap(QString fileName)
{
	return m_mapManager->openMap(fileName, 100);
}

void Map::onMapReady()
{
	QMap<int, IMapClient *>::iterator it;

	for (it = m_mapClients.begin(); it != m_mapClients.end(); ++it)
	{
		if(!it.value())
			continue;
		(it.value())->set_Point();
	}

	emit modelMapReady();
}

IMapClient* Map::getMapClient(int id)
{
	return m_mapClients[id];
}
