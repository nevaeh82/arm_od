#ifndef BPLAABSTRACTFEATURE_H
#define BPLAABSTRACTFEATURE_H

#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/Path.h>

#include "Tabs/DbBla/Defines.h"
#include "Map/Features/MarkerFeature.h"

namespace MapFeature {

/// Abstract BPLA feature representation on map
class BplaAbstract : public Marker
{
protected:
	Path* m_tail;
	Path* m_slices;

	double m_altitude;
	double m_speed;
	double m_angle;

	bool m_possible;
	bool m_initialized;

	BplaAbstract(IObjectsFactory* factory, const QString& id, const UavInfo& uav);

public:
	virtual ~BplaAbstract();

	virtual void setPosition(const QPointF &position);

	void setAltitude(double altitude);
	inline double altitude() { return m_altitude; }

	void setSpeed(double speed);
	inline double speed() { return m_speed; }

	void setAngle(double angle);
	inline double angle() { return m_angle; }

	void setPossible(bool value);
	inline bool possible() { return m_possible; }

	void setSlice(const QPointF &slice);

	void update(const UavInfo& uav);

	virtual void updateMap();
	virtual void removeFromMap();

protected:
	void registerStyle();

	virtual inline QString getStyleName() const = 0;
	virtual inline QString getSlicesStyleName() const = 0;
	virtual void updateName() {}
};

} // namespace MapFeature

#endif // BPLAABSTRACTFEATURE_H
