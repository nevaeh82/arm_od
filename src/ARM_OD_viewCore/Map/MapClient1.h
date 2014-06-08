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

#include "UAV/ZInterception.h"
#include "Map/IMapClient.h"
#include "Map/IMapStyleManager.h"
#include "Map/Features/FeaturesFactory.h"


class MapClient1 : public QObject, public IMapClient
{
	Q_OBJECT

private:
	IMapStyleManager* m_styleManager;
	MapFeature::IFeaturesFactory* m_factory;
	Pw::Gis::IMapBounds* m_bounds;
	PwGisWidget* m_pwWidget;

	Station* m_station;
	ZInterception* m_interception;

	QMap<int, QString> m_mapLayers;

	QMutex m_mux;

	// Map features
	MapFeature::NiippPoint* m_niippPoint;
	QMap<QString, MapFeature::FriendBpla*> m_friendBplaList;
	QMap<QString, MapFeature::EnemyBpla*> m_enemyBplaList;
	QMap<int, MapFeature::Niipp*> m_niippList;
	QMap<int, MapFeature::Pelengator*> m_pelengatorList;
	QMap<int, MapFeature::PelengatorPoint*> m_pelengatorPointsList;
	QMap<QString, MapFeature::Interception*> m_interceptionList;
	QMap<QString, MapFeature::Ais*> m_aisList;
	QMap<QString, MapFeature::Station*> m_stationList;
	QList<MapFeature::CheckPoint*> m_checkPointsList;
	QList<MapFeature::Hyperbole*> m_hyperboleList;

	QSet<uint> m_uavKnownSources;

	QTimer m_bplaRedrawTimer;

	/// \todo: I don't what for this variables
	QMap<int, int> m_mapBattle;
	QTimer* m_uiTimer;
	double m_circleRadius;
	bool m_circleChanged;

public:
	MapClient1( PwGisWidget* pwwidget, Station* station, QObject* parent = NULL );
	virtual ~MapClient1();

	virtual void showLayer( int index, bool state );

	virtual void addInterception(int blaId, int bplaId , QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList);
	virtual void removeInterception( int blaId, int bplaId );

	virtual void addPerehvatPoint( int blaId, int bplaId, QPointF coord, float hgt, float radius, int time, float intcCourse, float intcSpeed );

private:
	void addMarkerLayer( int id, const QString& layerId, const QString& name );

	void readStationsFromFile(QString fileName);
	void readCheckPointsFromFile(QString fileName);

	QString getUavInternalId(const UavInfo& uav);
	QString getUavInternalId(const Uav& uav, uint source);

public slots:
	virtual void init();
	virtual void setPoint();
	virtual void centerMap();
	virtual void justifyMap();

	virtual void addFriendBpla( const UavInfo& uav );
	virtual void addEnemyBpla( const UavInfo& uav,
							   const QVector<QPointF> &tail = QVector<QPointF>(),
							   const QVector<QPointF> &tailStdDev = QVector<QPointF>() );
	virtual void removeBpla( const Uav &uav );

	virtual void addAis( QMap<int, QVector<QString> > vec );

	virtual void addNiippPoint( const QPointF& point );

	virtual void updateNiippPowerZone( const Niipp& niipp );

	virtual void updatePeleng( int id, int idPost, double lat, double lon, double direction );

	virtual void removeNiippPoint();

	virtual void addHyperboles( const QByteArray& data, const QColor color  = QColor::Invalid );

	virtual void removeAll();

private slots:
	void addFriendBplaInternal( const UavInfo& uav );
	void addEnemyBplaInternal( const UavInfo& uav,
							   const QVector<QPointF> &tail = QVector<QPointF>(),
							   const QVector<QPointF> &tailStdDev = QVector<QPointF>() );
	void removeBplaInternal( const Uav& uav );

	void setAisData(QMap<int, QVector<QString> > data );

	void addPeleng( int id, int postId, double lat, double lon, double direction );

	void addNiipInternal( const Niipp& niipp );

	void addPerehvatData(int bla_id, int bpla_id , QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList);
	void removeInterceptionData(int friendBplaId, int enemyBplaId );
	void addInterceptionPointData(int friendBplaId, int enemyBplaId, QPointF position,
								  float height, float radius, int time,
								  float course, float speed );
	void addHyperboleInternal( const QByteArray& data, const QColor color );

	void redrawAllBpla();
	void updateCircle();

signals:
	void friendBplaAdded( const UavInfo& uav );
	void enemyBplaAdded( const UavInfo& uav,
						 const QVector<QPointF> &tail = QVector<QPointF>(),
						 const QVector<QPointF> &tailStdDev = QVector<QPointF>() );
	void bplaRemoved( const Uav& uav );

	void aisAdded( QMap<int, QVector<QString> > vec );

	void niippPowerZoneUpdated( const Niipp& );

	void pelengUpdated( int id, int post_id, double lat, double lon, double direction );

	void interceptionAdded( int blaId, int bplaId, QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList );
	void interceptionRemoved( int bla_id, int bpla_id );

	void interceptionPointAdded( int bla_id, int bpla_id, QPointF aCoord,
		float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed );

	void hyperboleAdded(const QByteArray& data, const QColor color);
};

#endif // MAPCLIENT1_H
