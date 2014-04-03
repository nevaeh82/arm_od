#ifndef MAPCLIENT1_H
#define MAPCLIENT1_H

#include <QApplication>
#include <QTextCodec>
#include <QSettings>
#include <QDebug>
#include <QMap>
#include <QTimer>
#include "IMapClient.h"
#include "../Tabs/TabsProperty.h"
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

#include "../Tabs/Tree/IDBManager.h"

#include "../UAV/ZInterception.h"

#include "MapFeatures/AisFeature.h"
#include "MapFeatures/NiippFeature.h"
#include "MapFeatures/PelengatorFeature.h"
#include "MapFeatures/PerehvatFeature.h"
#include "MapFeatures/BlaFeature.h"
#include "MapFeatures/BplaFeature.h"
#include "MapFeatures/StationFeature.h"


class MapClient1 : public IMapClient
{
	Q_OBJECT

public:
	MapClient1( PwGisWidget* pwwidget, TabsProperty* property,
		IDBManager* dbBla, IDBManager* dbEvil );
	~MapClient1();

	virtual void setNiippController( INiiPPController* niippController );

	virtual void showLayer( int index, bool state );

	virtual void addPerehvat( int blaId, int bplaId );
	virtual void removePerehvat( int blaId, int bplaId );

	virtual void addPerehvatPoint( int blaId, int bplaId, QPointF coord,
		float hgt, float radius, int time, float intcCourse, float intcSpeed );

	virtual void removePointUvoda();

private:
	IDBManager* m_dbBla;
	IDBManager* m_dbEvil;

	int m_niippLayerId;
	QMap<int, IMapObjectInfo*> m_mapObjects;

	ZInterception* m_perehvat;

	QMap<int, INiiPPController* > m_mapNiippController;

	PwGisWidget* m_pwwidget;
	Pw::Gis::IMapBounds* m_mapBounds;
	TabsProperty* m_property;

	QMap<int, QByteArray> m_mapCurPoints;

	QMap<int, int> m_mapBattle;

	QPointF m_pointUvodaNiipp;

	double m_mainLatitude;
	double m_mainLongitude;
	QMap< int, PwGisPointList* > *m_lastCoord;

	QMutex m_mux;

	QTimer* m_uiTimer;
	QTimer* m_uiTimerSlice;
	QTimer* m_updateTimer;
	QTimer* m_TimerSimulator;

	QMap<int, QString> m_mapLayers;
	Pw::Gis::ILayerManager* m_layerManager;
	int m_layersCounter;

	double m_circleRadius;
	bool m_circleChanged;
	bool m_sliceChanged;

	QString m_niippLayerName;

	AisFeature* m_aisFeature;
	NiippFeature* m_niippFeature;
	PelengatorFeature* m_PelengatorFeature;
	PerehvatFeature* m_PerehvatFeature;
	BlaFeature* m_BlaFeature;
	BplaFeature* m_BplaFeature;
	StationFeature* m_stationFeature;

	void addNiippLayer( QString id );
	void addMarkerLayer( int id, QString name );
	void removeAis();

public slots:
	virtual void setPoint();
	virtual void centerMap();
	virtual void justifyMap();

	void addBLA( int id, QByteArray data );
	void addEvil( int id, QByteArray data );
	void addAis( QMap<int, QVector<QString> > vec );

	virtual void updateNiippPowerSector( int id, double radius, double bis, QByteArray ba );
	void updateNiippPowerCicle( int id, double radius, QByteArray ba );

	virtual void updatePeleng( int id, int idPost, double lat, double lon, double direction );

private slots:
	void setPointBla( int id, QPointF point, double alt, double speed, double course, int state );
	void setPointEvil( int id, QByteArray data );
	void setPointEvilPeleng( int id, QPointF point );

	void setCurrentPoint( int id, QByteArray data );
	void setAisData(QMap<int, QVector<QString> > map );
	void updatePoints();
	void updatePelengData( int id, int postId, double lat, double lon, double direction );
	void updateSector( int id, double radius, double bis, QByteArray ba );
	void updateCicle( int id, double radius, QByteArray ba );

	void addPerehvatData( int bla_id, int bpla_id );
	void removePerehvatData(int blaId, int bplaId );
	void addPerehvatPointData( int blaId, int bplaId, QPointF coord,
		float hgt, float radius, int time, float intcCourse, float intcSpeed );

	void updateCircle();
	void updateSlice();
	void onMapReady();

	void mapMouseClicked( double, double );

signals:
	void friendBplaAdded( int id, QByteArray data );
	void enemyBplaAdded( int id, QByteArray data );
	void aisAdded( QMap<int, QVector<QString> > vec );

	void cicleUpdated( int, double, QByteArray );
	void sectorUpdated( int,double,double, QByteArray );
	void pelengUpdated( int id, int post_id, double lat, double lon, double direction );

	void perehvatAdded( int blaId, int bplaId );
	void perehvatRemoved( int bla_id, int bpla_id );

	void signalAddPerehvatPoint( int bla_id, int bpla_id, QPointF aCoord,
		float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed );
};

#endif // MAPCLIENT1_H
