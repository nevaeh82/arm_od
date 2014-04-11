#ifndef NIIPPFEATURE_H
#define NIIPPFEATURE_H

#include <PwGis/objects/Circle.h>
#include <PwGis/objects/Sector.h>

#include "Map/Features/FeatureAbstract.h"

namespace MapFeature {

class FeaturesFactory;

/// Niipp feature representation on map
class Niipp : public FeatureAbstract
{
	friend class FeaturesFactory;

public:
	enum Mode { Directed, NotDirected };

protected:
	Circle* m_circle;
	Sector* m_sector;

	int m_niippId;
	Mode m_mode;
	double m_radius;
	double m_angle;

	Niipp(IObjectsFactory* factory, const QString& id, int niippId, const QPointF& position,
				 Mode mode, double radius, double angle = 0);

public:
	virtual ~Niipp();

	virtual void setPosition(const QPointF &position);

	void setMode(Mode mode);
	/// \param radius In projection EPSG:900913 is pseudo meters. \
	///					Must use the projection EPSG:28406,28407...; EPSG:32636,32637...
	/// \link http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
	void setRadius(double value);
	void setAngle(float value);

	inline Mode mode() { return m_mode; }
	inline float radius() { return m_radius; }
	inline float angle() { return m_angle; }

	void update( const QPointF& position, Mode mode, double radius, double angle = 0 );

	virtual void updateMap();
	virtual void removeFromMap();
};

} // namespace MapFeature

#endif // NIIPPFEATURE_H
