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

#include "../Tabs/TabsProperty.h"

#include "MapClient1.h"
#include "IMapController.h"

#include "../Tabs/Tree/IDBManager.h"

#include "../UAV/ZInterception.h"

class MapControllerWidget;

class MapController : public QObject, public IMapController
{
	Q_OBJECT

public:
	MapController();
	~MapController();
	void init(QMap<int, TabsProperty *> map_settings, IDBManager* db_bla, IDBManager* db_evil);

	PwGisWidget *get_pwwidget();
	QWidget     *get_widget();
	QWidget     *get_panel_widget();

	virtual IMapClient  *get_map_client(int id);

	void addMarkerLayer(int id, QString name);

private:
	MapControllerWidget* m_controllerWidget;
	Map*                 m_mapModel;

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
	void controllerShowBLAtree();
	void controllerShowBPLAtree();
	void controllerShowNIIPP();

};

#endif // MAPCONTROLLER_H
