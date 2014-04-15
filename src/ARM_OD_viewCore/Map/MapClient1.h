#ifndef MAPCLIENT1_H
#define MAPCLIENT1_H

#include <math.h>
#include <stdlib.h>

#include <qmath.h>
#include <QApplication>
#include <QTextCodec>
#include <QSettings>
#include <QDebug>
#include <QMap>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>

#include <MapProvider.h>
#include <PwGis/ILayerManager.h>
#include <PwGis/LayerManager.h>
#include <PwGis/MapManagerEvents.h>
#include <PwGis/GeoRectangle.h>

#include "Station.h"

#include "NIIPP/INiippController.h"

#include "UAV/ZInterception.h"

#include "Map/IMapStyleManager.h"
#include "Map/Features/FeaturesFactory.h"

#include "IMapClient.h"

class MapClient1 : public QObject, public IMapClient
{
	Q_OBJECT

public:
	MapClient1( PwGisWidget* pwwidget, Station* station, QObject* parent = NULL );
	~MapClient1();

	virtual void setNiippController( INiiPPController* niippController );

	virtual void showLayer( int index, bool state );

	virtual void addInterception( int blaId, int bplaId );
	virtual void removeInterception( int blaId, int bplaId );

	virtual void addPerehvatPoint( int blaId, int bplaId, QPointF coord,
		float hgt, float radius, int time, float intcCourse, float intcSpeed );

	virtual void removePointUvoda();

private:
	IMapStyleManager* m_styleManager;
	MapFeature::FeaturesFactory* m_factory;

	QMap<int, IMapObjectInfo*> m_mapObjects;

	ZInterception* m_perehvat;

	QMap<int, INiiPPController* > m_mapNiippController;

	PwGisWidget* m_pwWidget;
	Pw::Gis::IMapBounds* m_mapBounds;
	Station* m_station;

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

	double m_circleRadius;
	bool m_circleChanged;
	bool m_sliceChanged;

	MapFeature::NiippPoint* m_niippPoint;
	QMap<int, MapFeature::FriendBpla*> m_friendBplaList;
	QMap<int, MapFeature::EnemyBpla*> m_enemyBplaList;
	QMap<int, MapFeature::Niipp*> m_niippList;
	QMap<int, MapFeature::Pelengator*> m_pelengatorList;
	QMap<int, MapFeature::PelengatorPoint*> m_pelengatorPointsList;
	QMap<QString, MapFeature::Interception*> m_interceptionList;
	QMap<QString, MapFeature::Ais*> m_aisList;
	QMap<QString, MapFeature::Station*> m_stationList;

	void addMarkerLayer(int id, const QString& layerId,
		const QString& name );

	void readStationsFromFile(QString fileName);

public slots:
	virtual void init();
	virtual void setPoint();
	virtual void centerMap();
	virtual void justifyMap();

	void addFriendBpla( int id, QByteArray data );
	void addEnemyBpla( int id, QByteArray data );
	void addAis( QMap<int, QVector<QString> > vec );

	virtual void updateNiippPowerSector( int id, double radius, double bis, QByteArray ba );
	void updateNiippPowerCicle( int id, double radius, QByteArray ba );

	virtual void updatePeleng( int id, int idPost, double lat, double lon, double direction );

private slots:
	void setPointBla( int id, QPointF point, double alt, double speed, double course, int state );
	void setPointEvil( int id, QByteArray data );
	void setPointEvilPeleng( int id, QPointF point );

	void setCurrentPoint( int id, QByteArray data );
	void setAisData(QMap<int, QVector<QString> > data );
	void updatePoints();
	void addPeleng( int id, int postId, double lat, double lon, double direction );

	void addNiippDirected(int id, double radius, double angle, QByteArray data );
	void addNiippNotDirected(int id, double radius, QByteArray data );
	void addNiip( int id, MapFeature::Niipp::Mode mode, double radius, QByteArray data,
				  double angle = 0 );

	void addPerehvatData( int bla_id, int bpla_id );
	void removeInterceptionData(int friendBplaId, int enemyBplaId );
	void addInterceptionPointData(int friendBplaId, int enemyBplaId, QPointF position,
								  float height, float radius, int time,
								  float course, float speed );

	void updateCircle();
	void updateSlice();

	void mapMouseClicked( double, double );

signals:
	void friendBplaAdded( int id, QByteArray data );
	void enemyBplaAdded( int id, QByteArray data );
	void aisAdded( QMap<int, QVector<QString> > vec );

	void cicleUpdated( int, double, QByteArray );
	void sectorUpdated( int,double,double, QByteArray );
	void pelengUpdated( int id, int post_id, double lat, double lon, double direction );

	void interceptionAdded( int blaId, int bplaId );
	void interceptionRemoved( int bla_id, int bpla_id );

	void interceptionPointAdded( int bla_id, int bpla_id, QPointF aCoord,
		float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed );
};

#endif // MAPCLIENT1_H
