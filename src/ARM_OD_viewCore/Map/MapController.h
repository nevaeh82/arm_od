#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>

#include <QMenu>
#include <QEvent>

#include <QMap>

#include <MapManager.h>
#include <pwgiswidget.h>

#include <IMapManager.h>
#include <IProfileManager.h>

#include "MapWidget.h"
#include "Map.h"

#include "Station.h"

#include "MapClient1.h"
#include "IMapController.h"

#include "../UAV/ZInterception.h"

#include <Interfaces/IController.h>

class MapWidget;

class MapController : public QObject, public IMapController, public IController<MapWidget>
{
	Q_OBJECT

public:
	MapController(QObject* parent =NULL);
	virtual ~MapController();
	void init(QMap<int, Station *> map_settings);

	virtual IMapClient  *getMapClient(int id);

	void addMarkerLayer(int id, QString name);

	void appendView(MapWidget* view);

	void closeMap();
	void closeAtlas();

private:
	MapWidget* m_view;
	Map* m_mapModel;


public slots:
	void openMapFromAtlas();
	void openMapFromLocalFile();
	void onMapReady();

private slots:
	void _slot_station_visible(bool state);

signals:
	void mapOpened();

};

#endif // MAPCONTROLLER_H
