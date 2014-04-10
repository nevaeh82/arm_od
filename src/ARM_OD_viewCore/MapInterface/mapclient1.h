#ifndef MAPCLIENT1_H
#define MAPCLIENT1_H

#include <QApplication>
#include <QTextCodec>
#include <QSettings>
#include <QDebug>
#include <QMap>
#include <QTimer>
#include "IMapClient.h"
#include "Station.h"
#include <QMutex>
#include <QMutexLocker>
#include <stdlib.h>

#include <qmath.h>
#include <QThread>

//#include <PwGis/IMapProvider.h>
#include <PwGis/ILayerManager.h>
#include <PwGis/LayerManager.h>
//#include <PwGis/objects/Sector.h>
//#include <PwGis/TacticMap/ITacticObjectsFactory.h>
//#include <PwGis/objects/IMapObjectInfo.h>
//#include <PwGis/objects/IObjectsFactory.h>
#include <math.h>

#include <MapProvider.h>
#include <IdGenerator.h>
#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/IconStyle.h>
#include <PwGis/objects/LineStyle.h>
#include <PwGis/objects/TextStyle.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/objects/Sector.h>
#include <PwGis/objects/Circle.h>
#include <PwGis/objects/Path.h>
#include <PwGis/objects/Polygon.h>
#include <PwGis/objects/IObjectsFactory.h>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/GeoRectangle.h>

#include "../NIIPP/INiippController.h"

#include "../UAV/ZInterception.h"

#include "UAVDefines.h"

//typedef struct tt
//{
//    QPointF *point;
//    double alt;
//    double speed;
//    double course;
//    int state;
//}tt;

//typedef struct niipp_point
//{
//    QString id;
//    QString caption;
//    PwGisLonLat pwll;

//}niipp_point;

class MapClient1 : public IMapClient
{
    Q_OBJECT
public:
	MapClient1(PwGisWidget *pwwidget, Station* station);
	virtual ~MapClient1();

    virtual void set_niipp_controller(INiiPPController *niipp_controller);

    virtual void show_layer(int index, bool state);

    virtual void add_perehvat(int bla_id, int bpla_id);
    virtual void remove_perehvat(int bla_id, int bpla_id);

    virtual void perehvat_point(int bla_id, int bpla_id, QPointF aCoord, float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed);

    virtual void slot_remove_point_uvoda();



private:
    /// layers

//    PwGisStyle*                 _pelengStyle;


private:
    void _add_niipp_layer(QString id);

    int _niipp_layer_id;
    QMap<int, IMapObjectInfo*>      _map_objects;

    Sector*                         _peleng;
    Sector*                         _sector;
    Circle*                         _cicle;

    QMap<int, Sector* >             _map_sector;
    QMap<int, Circle* >             _map_circle;

//    QMap<int, int>              _map_peleng_id;
    QMap<QString, PwGisLonLat *>         _map_peleng;
    QMap<QString, PwGisLonLat *>         _map_niip_sector;
    QMap<QString, PwGisLonLat *>         _map_niip_circle;

	QMap<int, QVector<QString> >		_map_ais;



    QMap<int, int>                      _map_peleng_point;

    ZInterception*                      _perehvat;


    QMap<int, INiiPPController* >               _map_niipp_Controller;
//    bool                        _ready;

    double  _bis123;


public slots:
    virtual void set_Point();
    virtual void center_map();
    virtual void justify_map();




//    void slotAddBLA_evil(int id_BE, QPointF point);
    void slot_add_BLA(int id, QByteArray data);
    void slot_add_evil(int id, QByteArray data);

    void slot_add_ais(QMap<int, QVector<QString> > vec);

    void slot_niipp_power_cicle(int id, double radius, QByteArray ba);


//    virtual void slot_create_niipp_sector(int id, QString name, double lon, double lat, double angel);
    virtual void slot_update_sector(int id, double radius, double bis, QByteArray ba);

    virtual void slot_peleng(int id, int id_post, double lat, double lon, double direction);
//    virtual void slot_peleng_evil(int id, double lat, double lon);
private slots:
	void _slot_set_point_bla(int id, const QPointF& point, double alt, double speed, double course, int state);
    void _slot_set_point_evil(int id, QByteArray data);
	void _slot_set_point_evil_peleng(int id, const QPointF& point);

    void _slot_set_ais_data(QMap<int, QVector<QString> > vec);

    void _slot_set_cur_point(int id, QByteArray data);
    void _slot_update_points();
    void _slot_simulator();

    void _slot_update_peleng(int id, int post_id, double lat, double lon, double direction);
//    void _slot_update_peleng_evil(int id, double lat, double lon);


    void _slot_update_sector(int id, double radius, double bis, QByteArray ba);
    void _slot_update_cicle(int id, double radius, QByteArray ba);

    void _slot_add_perehvat(int bla_id, int bpla_id);
    void _slot_remove_perehvat(int bla_id, int bpla_id);

    void _slot_perehvat_point(int bla_id, int bpla_id, QPointF aCoord, float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed);


    void updateCircle();
    void updateSlice();
    void onMapReady();

    void _slot_mouse_clicked(double, double);
private:
    PwGisWidget             *_pwwidget;
    Pw::Gis::IMapBounds     *_mapBounds;

	Station*					m_station;

    QMap<int, QByteArray>      _map_cur_points;

    QMap<int, int>          _map_battle;

    QPointF                 _point_uvoda_niipp;

    double _main_latitude;
    double _main_longitude;
    QMap<int, PwGisPointList *>  *_last_coord;
//    PwGisPointList *p;
//    QVector<PwGisLonLat> vec;

//    double lat1;
//    double lon1;
    QMap<int, QVector<int> *>              _map_path;
    QMap<int, QVector<QPointF> *>          _map_path_line;
    int                                     _line_id;
//    bool            _temp;


    QMutex _mux;

    QTimer                                  *_uiTimer;
    QTimer                                  *_uiTimerslice;
    QTimer                                  *_update_timer;
    QTimer                                  *_timer_simulator;



    QMap<int, QString>          _map_layers;
    Pw::Gis::ILayerManager      *_layerManager;
    int                         _layersCounter;



    int _rad;

    double _circleRadius;
    bool   _circleChanged;
    bool   _circleExists;

    double _sliceRadius;
    bool   _sliceChanged;
    bool   _sliceExists;

//    double _radius;
    double _bis;


    QString                                 _niipp_layer_name;

signals:
    void signalNewBPLA(QString bpla);
    void signal_new_coord_bla(int id, QByteArray data);
    void signal_new_coord_evil(int id, QByteArray data);
    void signalUpdateTumeOut();
    void signalTimeoutSimulator();

    void signal_new_ais_data(QMap<int, QVector<QString> > vec);

    void signalUpdateCicle(int, double, QByteArray);
    void signalUpdateSector(int,double,double, QByteArray);


    void signal_peleng(int id, int post_id, double lat, double lon, double direction);
    void signal_peleng_evil(int id, double lat, double lon);

    void signalAddPerehvat(int bla_id, int bpla_id);
    void signalRemovePerehvat(int bla_id, int bpla_id);

    void signalPerehvatPoint(int bla_id, int bpla_id, QPointF aCoord, float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed);



private:
    int _read_settings(QString path_to_ini_file);

    int _read_settings1(QString path_to_ini_file);


    void _addMarkerLayer(int id, QString name);

	void _remove_ais();



};

#endif // MAPCLIENT1_H
