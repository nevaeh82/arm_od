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

#include "../Tabs/Tree/IDBManager.h"

#include "../UAV/ZInterception.h"

#include <Interfaces/IController.h>

class MapWidget;

class MapController : public QObject, public IMapController, public IController<MapWidget>
{
	Q_OBJECT

public:
	MapController(QObject* parent =NULL);
	~MapController();
	void init(QMap<int, Station *> map_settings, IDBManager* db_bla, IDBManager* db_evil);

	PwGisWidget *get_pwwidget();
	QWidget     *get_widget();

	virtual IMapClient  *get_map_client(int id);

	void addMarkerLayer(int id, QString name);

	void appendView(MapWidget* view);

private:
	MapWidget* m_view;
	Map* m_mapModel;

private:
	bool eventFilter(QObject *obj, QEvent *e);

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
