#ifndef MAP_H
#define MAP_H

#include <pwgiswidget.h>
#include <IMapManager.h>
#include <IProfileManager.h>

#include "IMapController.h"
#include "Station.h"

#include "MapClient1.h"

#include "BplaSimulator.h"

#define MAPOBJECTS_CACHE "./Map/objectsCache.geojson"
#define MAPMARKERS_CACHE "./Map/markersCache.geojson"

class Map : public QObject
{
	Q_OBJECT

public:
	Map(QObject* parent = NULL);
	~Map();

	void init(QMap<int, Station *> map_settings, MapWidget* pwwidget);

	void setMapManager(Pw::Gis::IMapManager* mapManager);
	void setProfileManager(Pw::Gis::IProfileManager* profileManager);
	bool openAtlas();
	bool openMap(QString fileName);
	void closeAtlas();
	void closeMap();

	void setLayerManager(Pw::Gis::ILayerManager* layerManager);
	void setStationVisible(bool state);
	IMapClient*getMapClient(int id);

	IMapClient *getFirstMapClient();
	void saveCache();
private:
	Pw::Gis::IMapManager        *m_mapManager;
	Pw::Gis::IProfileManager    *m_profileManager;

	QMap<int, IMapClient *>     m_mapClients;

    IMapClient*     m_firstClient;

	QMap<int, Station*>   m_mapSettings;

	QMap<int, QString>          _map_layers;
	Pw::Gis::ILayerManager      *m_layerManager;
	int                         m_layersCounter;

	MapWidget* m_pwWidget;
    BplaSimulator* m_simulator;

private slots:
	void onMapReady();

signals:
	void modelMapReady();
    void onSquare(QString);
};

#endif // MAP_H
