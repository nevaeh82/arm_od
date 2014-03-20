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

#include "MapControllerWidget.h"

#include "../Tabs/TabsProperty.h"

#include "MapClient1.h"
#include "IMapController.h"

#include "../Tabs/Tree/IDBManager.h"

#include "../UAV/ZInterception.h"

class MapControllerWidget;

class MapController : public QWidget, public IMapController
{
    Q_OBJECT
public:
//    static MapController* getInstance();

    ~MapController();
    void init(QMap<int, TabsProperty *> map_settings, IDBManager* db_bla, IDBManager* db_evil);
//    void init2();

    PwGisWidget *get_pwwidget();
    QWidget     *get_widget();
    QWidget     *get_panel_widget();

    virtual IMapClient  *get_map_client(int id);

    void addMarkerLayer(int id, QString name);



public:
    MapController();

//    static MapController* _map_controller;




public:

private:
    QCheckBox*                  _chb_stations;

    IDBManager*                 _db_bla;
    IDBManager*                 _db_evil;





private:
    Pw::Gis::IMapManager        *_mapManager;
    Pw::Gis::IProfileManager    *_profileManager;
	MapControllerWidget         *_controller_widget;


    QMap<int, IMapClient *>     _map_clients;
    QMap<int, TabsProperty *>   _m_settings;

    QMap<int, QString>          _map_layers;
    Pw::Gis::ILayerManager      *_layerManager;
    int                         _layersCounter;



private:
    bool eventFilter(QObject *obj, QEvent *e);

public slots:
    void open_map_from_Atlas(/*QString path*/);
    void open_map_from_local_file(/*const QString mapFile*/);
    void onMapReady();

private slots:
    void _slot_station_visible(bool state);

signals:
    void signalOpenedMap();

	void controller_showBLAtree();
	void controller_showBPLAtree();
	void controller_showNIIPP();

};

#endif // MAPCONTROLLER_H
