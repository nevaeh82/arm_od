#pragma once

#include "QTimer"
#include <PwGis/objects/Circle.h>
#include <PwGis/objects/Sector.h>
#include <PwGis/objects/Marker.h>

#include "Map/Features/FeatureAbstract.h"

#include "Niipp/nstation.h"
#include "Tcp/TcpClientManager.h"

#include "Tabs/DbBla/Defines.h"

namespace MapFeature {

class FeaturesFactory;

class PvoFeature : public FeatureAbstract
{
    Q_OBJECT

	friend class FeaturesFactory;

public:
    enum Mode { Pvo, Reb };

protected:
	Pw::Gis::Circle* m_circle;
	Pw::Gis::Sector* m_sector;
    Pw::Gis::Marker* m_marker;

	int m_niippId;
	Mode m_mode;
	double m_radius;
	double m_angle;

    NStation m_params;

    PvoFeature(IObjectsFactory* factory, PwGisWidget* pw, const NStation& niipp,
                                                             PwGisStyle* styleSimplePvo,
                                                             PwGisStyle* styleActivePvo,
                                                             PwGisStyle* styleSimpleReb,
                                                             PwGisStyle* styleActiveReb);

public:
    virtual ~PvoFeature();

    virtual void setPosition(const QPointF &position);

	void setMode(Mode mode);
	/// \param radius In projection EPSG:900913 is pseudo meters. \
	///					Must use the projection EPSG:28406,28407...; EPSG:32636,32637...
	/// \link http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
    void setRadius(double value);
    //void setAngle(double value);

	inline Mode mode() { return m_mode; }
	inline float radius() { return m_radius; }
	inline float angle() { return m_angle; }

    void update(const NStation& niipp);

    void setEnemyPoint(QString, const UavInfo &uav);

	virtual void updateMap();
    virtual void removeFromMap();
    void checkIntersectons();

    void setTcpClientManager(TcpClientManager *manager);

private slots:
     void onBlinkTimeout();
     void onBlinking();

private:
    void setAzimuth(int azimuth, int width);
    PwGisWidget* m_pwgis;
    double getCorrectDegAngle(const double deg_1);
    double RadianToDegree(double angle);
    double DegreeToRadian(double angle);
    QPointF getPointByDistance(double lon1, double lat1, double dist, double azimuth);

    QMap<QString, UavInfo> m_enemyPointList;
    QTimer* m_animationTimer;
    PwGisStyle* m_styleSimplePvo;
    PwGisStyle* m_styleActivePvo;
    PwGisStyle* m_styleSimpleReb;
    PwGisStyle* m_styleActiveReb;

    QPolygonF m_myPolygon;
    QString m_name;

    QTimer *m_blinkTimer;
    QTimer *m_styleBlinkTimer;

    bool m_isBlink;
    bool m_isIntersect;

    TcpClientManager* m_tcpClientManager;
    double getAzimuthOnSphere(const double lon1, const double lat1, const double lon2, const double lat2);
    double getDistanceOnSphere(double lon1, double lat1, double lon2, double lat2);
    double getAzimuth(const double x1, const double y1, const double x2, const double y2);
};

} // namespace MapFeature
