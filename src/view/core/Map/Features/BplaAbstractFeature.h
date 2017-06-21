#ifndef BPLAABSTRACTFEATURE_H
#define BPLAABSTRACTFEATURE_H

#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/Path.h>
#include <PwGis/objects/Polygon.h>

#include "Tabs/DbBla/Defines.h"
#include "Map/Features/MarkerFeature.h"

namespace MapFeature {

/// Abstract BPLA feature representation on map
class BplaAbstract : public Marker
{
protected:
	uint m_sourceId;

	Pw::Gis::Path* m_tail;
	Pw::Gis::Path* m_slices;
    Pw::Gis::Polygon* m_ellipse;

	double m_altitude;
	double m_speed;
	double m_angle;

	double m_lattitudeStddev;
	double m_longtitudeStddev;

	bool m_possible;
	bool m_initialized;
	bool m_isHistorical;
	bool m_tailEnabled;
	bool m_autoTail;

    QDateTime m_lastDateTime;

	BplaAbstract(IObjectsFactory* factory, const QString& id, const UavInfo& uav);

public:
	virtual ~BplaAbstract();

	virtual void setPosition(const QPointF &position);
	virtual void setTail(const QVector<QPointF> &tail, const QVector<QPointF> &tailStdDev = QVector<QPointF>());

	void setAltitude(double altitude);
	inline double altitude() { return m_altitude; }

	void setSpeed(double speed);
	inline double speed() { return m_speed; }

	void setAngle(double angle);
	inline double angle() { return m_angle; }

	void setPossible(bool value);
	inline bool possible() { return m_possible; }

	void setLattitudeStddev(double lattitude);
	inline double lattitudeStddev() { return m_lattitudeStddev; }

	void setLongtitudeStddev(double longtitude);
	inline double longtitudeStddev() { return m_longtitudeStddev; }

	void setSlice(const QPointF &slice);
    void setEllipse(PwGisPointList list);


	void update(const UavInfo& uav);

	virtual void updateMap();
	virtual void removeFromMap();

	int cource();

protected:
	void registerStyle();

	virtual inline QString getStyleName() const = 0;
	virtual inline QString getTrackStyleName() const = 0;
	virtual inline QString getErrorStyleName() const = 0;
	virtual inline QString getSliceStyleName() const = 0;
	virtual void updateName() {}
};

} // namespace MapFeature

#endif // BPLAABSTRACTFEATURE_H
