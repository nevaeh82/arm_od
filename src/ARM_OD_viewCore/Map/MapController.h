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

#include "Station.h"

#include "Map/Map.h"
#include "Map/MapWidget.h"
#include "Map/MapClient1.h"
#include "Map/IMapController.h"

#include "Tabs/Tree/IDBManager.h"

#include "UAV/ZInterception.h"

class MapWidget;

class MapController : public QObject, public IMapController
{
	Q_OBJECT

public:
	MapController();
	~MapController();
	void init(QMap<int, Station*> stations);

	PwGisWidget* getPwWidget();
	QWidget* getWidget();
	QWidget* getPanelWidget();

	virtual IMapClient* getMapClient( int id );

	void addMarkerLayer( int id, QString name );

	void appendView(MapWidget* view);

private:
	MapWidget* m_view;
	Map* m_map;

public slots:
	void openMapFromAtlas();
	void openMapFromLocalFile();
	void onMapReady();

private slots:
	void setStationVisibility( bool state );

signals:
	void mapOpened();
	void showFriendBplaTreeCliecked();
	void showEnemyBplaTreeClicked();
	void showNiippClicked();
};

#endif // MAPCONTROLLER_H
