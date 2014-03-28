#ifndef MAP_H
#define MAP_H

#include <pwgiswidget.h>
#include <IMapManager.h>
#include <IProfileManager.h>

#include "IMapController.h"
#include "../Tabs/Tree/IDBManager.h"
#include "../Tabs/TabsProperty.h"

#include "MapClient1.h"


class Map : public QObject
{
	Q_OBJECT

public:
	Map();
	~Map();

	void init( QMap<int, TabsProperty *> map_settings, IDBManager* db_bla, IDBManager* db_evil, PwGisWidget* pwwidget );

	void setMapManager( Pw::Gis::IMapManager* mapManager );
	void setProfileManager( Pw::Gis::IProfileManager* profileManager );
	bool openAtlas();
	bool openMap( QString fileName );
	void setLayerManager( Pw::Gis::ILayerManager* layerManager );
	void setStationVisible( bool state );
	IMapClient*getMapClient( int id );

private:
	IDBManager* m_dbBla;
	IDBManager* m_dbEvil;

	Pw::Gis::IMapManager* m_mapManager;
	Pw::Gis::IProfileManager* m_profileManager;

	QMap<int, IMapClient *> m_mapClients;
	QMap<int, TabsProperty *> m_mapSettings;

	QMap<int, QString> _map_layers;
	Pw::Gis::ILayerManager* m_layerManager;
	int m_layersCounter;

private slots:
	void onMapReady();

signals:
	void modelMapReady();
};

#endif // MAP_H
