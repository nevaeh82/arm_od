#ifndef PELENGATORFEATURE_H
#define PELENGATORFEATURE_H

#include <PwGis/objects/Sector.h>

#include "Map/Features/FeatureAbstract.h"

namespace MapFeature {

class FeaturesFactory;

/// Pelengator feature representation for map
class Pelengator : public FeatureAbstract
{
	friend class FeaturesFactory;

protected:
	Sector* m_sector;
	int m_postId;
	double m_angle;

	Pelengator(IObjectsFactory* factory, const QString& id, int postId,
			   const QPointF& position, double angle );

public:
	virtual ~Pelengator();

	virtual void setPosition(const QPointF &position);

	void setAngle(double angle);
	inline float angle() { return m_angle; }

	void update( const QPointF& position, double angle = 0 );

	virtual void updateMap();
	virtual void removeFromMap();
};

} // namespace MapFeature

#endif // PELENGATORFEATURE_H
