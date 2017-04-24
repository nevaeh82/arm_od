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

#include "ADSBPacket.pb.h"


class MapClient1 : public QObject, public IMapClient
{
	Q_OBJECT

private:
	Pw::Gis::Circle* m_captureCircle;

	IMapStyleManager* m_styleManager;
	MapFeature::IFeaturesFactory* m_factory;
	Pw::Gis::IMapBounds* m_bounds;
	Pw::Gis::IObjectsManager* m_objectsManager;
	PwGisWidget* m_pwWidget;
	MapWidget* m_view;

	Station* m_station;
	ZInterception* m_interception;

	QMap<int, QString> m_mapLayers;

	QMutex m_mux;

	// Map features
	MapFeature::NiippPoint* m_niippPoint;
	QMap<QString, MapFeature::FriendBpla*> m_friendBplaList;
	QMap<QString, MapFeature::EnemyBpla*> m_enemyBplaList;
	QList<MapFeature::EnemyBpla*> m_onePointList;
	QMap<int, MapFeature::Niipp*> m_niippList;
	QMap<int, MapFeature::Pelengator*> m_pelengatorList;
	QMap<int, MapFeature::PelengatorPoint*> m_pelengatorPointsList;
	QMap<QString, MapFeature::Interception*> m_interceptionList;
	QMap<QString, MapFeature::Ais*> m_aisList;
	QMap<QString, MapFeature::ADSBPlaneFeature*> m_adsbList;
	QMap<QString, MapFeature::Station*> m_stationList;

	QList<MapFeature::CheckPoint*> m_checkPointsList;
	QList<QPair<QPointF, QPointF>> m_linesList;

	QList<MapFeature::Hyperbole*> m_hyperboleList;

	QSet<uint> m_uavKnownSources;

	QTimer m_bplaRedrawTimer;

	/// \todo: I don't what for this variables
	QMap<int, int> m_mapBattle;
	QTimer* m_uiTimer;
	double m_circleRadius;
	bool m_circleChanged;

	QTimer* m_hyperboleTimer;
	QTimer* m_onePointTimer;

    uint m_pointKKlastInd;
    uint m_ellipseCounter;
    QList<QString> m_trajEllipseCounter;
	QList<Pw::Gis::Marker*> m_baseStationMarkerList;

public:
	MapClient1( MapWidget* pwwidget, Station* station, QObject* parent = NULL );
	virtual ~MapClient1();

	virtual void showLayer( int index, bool state );

	virtual void addInterception(int blaId, int bplaId , QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList);
	virtual void removeInterception( int blaId, int bplaId );

	virtual void addPerehvatPoint( int blaId, int bplaId, QPointF coord, float hgt, float radius, int time, float intcCourse, float intcSpeed );

private:
	void addMarkerLayer( int id, const QString& layerId, const QString& name );

	void readStationsFromFile(QString fileName);
	void readCheckPointsFromFile(QString fileName);
	void readCheckLinesFromFile(QString fileName);

    //Strange name, hard work in DZankoi
    void DrawHyerboles(QList<QVector<QPointF>> list, QTime time, const QColor color);
	void DrawHyerboles(const QList<QVector<QPointF> >& list,
					   const QList<QVector<QPointF> >& zone, const QTime& time, const QColor& color);

    void clearEllipse();
    void clearHyperbole();
    void clearKKPoint();
	void clearTrajEllipse(QString id);
	QPointF drawAim(QPointF pos, int angle);
public slots:
	virtual void init();
	virtual void setPoint();
	virtual void centerMap();
	virtual void justifyMap();

	virtual void addFriendBpla( const UavInfo& uav );
    virtual void addEnemyBpla( const UavInfo& uav,
                               const QVector<QPointF> &tail = QVector<QPointF>(),
                               const QVector<QPointF> &tailStdDev = QVector<QPointF>() );

    virtual void addSingleMark( const QByteArray& uav );
	virtual void addTrajectoryKK( const QByteArray& uav, const int source = 100 );

    virtual void addStation( const QByteArray& data );
    virtual void addWorkArea( const QPointF& point1, const QPointF& point2 );

    virtual void clearSolver1();

    virtual void removeBpla( const Uav &uav );

	virtual void addAis( QMap<int, QVector<QString> > vec );

	virtual void addAdsb( QByteArray data );

	virtual void addNiippPoint( const QPointF& point );

	virtual void updateNiippPowerZone( const Niipp& niipp );

	virtual void updatePeleng( int id, int idPost, double lat, double lon, double direction );

	virtual void removeNiippPoint();

    virtual void addHyperboles( const QByteArray& data, int version = 0, const QColor color  = QColor::Invalid );

	virtual void removeAll();

private slots:
	void addFriendBplaInternal( const UavInfo& uav );
	void addEnemyBplaInternal( const UavInfo& uav,
							   const QVector<QPointF> &tail = QVector<QPointF>(),
							   const QVector<QPointF> &tailStdDev = QVector<QPointF>() );

    void addSingleMarkInternal(QByteArray data);

	void removeBplaInternal( const Uav& uav );

	void setAisData(QMap<int, QVector<QString> > data );
	void setAdsbData( QByteArray data );

	void addPeleng( int id, int postId, double lat, double lon, double direction );

	void addNiipInternal( const Niipp& niipp );

	void addPerehvatData(int bla_id, int bpla_id , QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList);
	void removeInterceptionData(int friendBplaId, int enemyBplaId );
	void addInterceptionPointData(int friendBplaId, int enemyBplaId, QPointF position,
								  float height, float radius, int time,
								  float course, float speed );
    void addHyperboleInternal(const QByteArray& data, int version, const QColor color );

    void addStationInternal(QByteArray data);
    void addAreaInternal(QPointF point1, QPointF point2);

	void removeAllHyperbole();
	void removeAllonePointTimer();

	void redrawAllBpla();
	void updateCircle();

	void removeAdsb(QString id);

	void onFeatureClicked(QString id, QString type);

	void addTrajectoryKKInternal(QByteArray data, int source);

	void slotSolverClear();

	void countCaptureApply(int);
	void slotCaptureClear();

	void onFeatureAdded(QString id, QString type);

	void slotAddBaseStation(double lon, double lat, QString name);
	void slotClearBaseStation();

signals:
	void friendBplaAdded( const UavInfo& uav );
	void enemyBplaAdded( const UavInfo& uav,
						 const QVector<QPointF> &tail = QVector<QPointF>(),
						 const QVector<QPointF> &tailStdDev = QVector<QPointF>() );

    void singleMarkAdded( QByteArray data );
	void trajectoryKKAdded( QByteArray data, int source );

	void bplaRemoved( const Uav& uav );

	void aisAdded( QMap<int, QVector<QString> > vec );

	void adsbAdded(QByteArray);

	void niippPowerZoneUpdated( const Niipp& );

	void pelengUpdated( int id, int post_id, double lat, double lon, double direction );

	void interceptionAdded( int blaId, int bplaId, QList<UavInfo>& blaInfoList, QList<UavInfo>& bplaInfoList );
	void interceptionRemoved( int bla_id, int bpla_id );

	void interceptionPointAdded( int bla_id, int bpla_id, QPointF aCoord,
		float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed );

    void hyperboleAdded(const QByteArray& data, int version, const QColor color);

    void signalAddStation(QByteArray);
    void signalAddArea(QPointF, QPointF);

    void onSolverClear();
};

#endif // MAPCLIENT1_H
