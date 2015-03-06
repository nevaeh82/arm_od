#ifndef MAP_H
#define MAP_H

#include <pwgiswidget.h>
#include <IMapManager.h>
#include <IProfileManager.h>

#include "IMapController.h"
#include "Station.h"

#include "MapClient1.h"

class Map : public QObject
{
	Q_OBJECT

public:
	Map(QObject* parent = NULL);
	~Map();

	void init(QMap<int, Station *> map_settings, PwGisWidget* pwwidget);

	void setMapManager(Pw::Gis::IMapManager* mapManager);
	void setProfileManager(Pw::Gis::IProfileManager* profileManager);
	bool openAtlas();
	bool openMap(QString fileName);
	void closeAtlas();
	void closeMap();

	void setLayerManager(Pw::Gis::ILayerManager* layerManager);
	void setStationVisible(bool state);
	IMapClient*getMapClient(int id);

private:
	Pw::Gis::IMapManager        *m_mapManager;
	Pw::Gis::IProfileManager    *m_profileManager;

	QMap<int, IMapClient *>     m_mapClients;
	QMap<int, Station*>   m_mapSettings;

	QMap<int, QString>          _map_layers;
	Pw::Gis::ILayerManager      *m_layerManager;
	int                         m_layersCounter;

private slots:
	void onMapReady();

signals:
	void modelMapReady();
};

#endif // MAP_H
