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

void Map::init(QMap<int, Station*> map_settings, PwGisWidget* pwwidget)
{
	m_mapSettings = map_settings;

	QMap<int, Station*>::iterator it;
	for (it = m_mapSettings.begin(); it != m_mapSettings.end(); ++it)
	{
		MapClient1* client = new MapClient1(pwwidget, it.value(), this);
		m_mapClients[(it.value())->id] = client;

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

		//=================
		//alax
		//1


		PwGisPointList* hyperbole1 = new PwGisPointList();
		hyperbole1->append( new PwGisLonLat( 30.25364, 59.98209 ) );
		hyperbole1->append( new PwGisLonLat( 30.32118, 59.97882 ) );
		hyperbole1->append( new PwGisLonLat( 30.34521, 59.95236 ) );
		hyperbole1->append( new PwGisLonLat( 30.39877, 59.95528 ) );
		it.value()->addHyperbole( 1, hyperbole1, QTime::currentTime() );

		//2
		PwGisPointList* hyperbole2 = new PwGisPointList();
		hyperbole2->append( new PwGisLonLat( 30.29749, 59.90849 ) );
		hyperbole2->append( new PwGisLonLat( 30.31912, 59.90832 ) );
		hyperbole2->append( new PwGisLonLat( 30.34349, 59.91676 ) );
		hyperbole2->append( new PwGisLonLat( 30.39499, 59.91899 ) );
		it.value()->addHyperbole( 2, hyperbole2, QTime::currentTime().addSecs(10), QColor::fromRgb( 255,0,0 ) );

		//3
		PwGisPointList* hyperbole3 = new PwGisPointList();
		hyperbole3->append( new PwGisLonLat( 30.49215, 59.89661 ) );
		hyperbole3->append( new PwGisLonLat( 30.46984, 59.92020 ) );
		hyperbole3->append( new PwGisLonLat( 30.47842, 59.94497 ) );
		hyperbole3->append( new PwGisLonLat( 30.45645, 59.96542 ) );
		hyperbole3->append( new PwGisLonLat( 30.45782, 59.98810 ) );
		it.value()->addHyperbole( 3, hyperbole3, QTime::currentTime(), QColor::fromRgb( 255, 0, 255 ) );

		delete hyperbole1;
		delete hyperbole2;
		delete hyperbole3;

		//=================




	}
}

IMapClient* Map::getMapClient(int id)
{
	return m_mapClients[id];
}
