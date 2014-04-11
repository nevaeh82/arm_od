#ifndef NIIPPPOINTFEATURE_H
#define NIIPPPOINTFEATURE_H

#include "Map/Features/MarkerFeature.h"

namespace MapFeature {

class FeaturesFactory;

/// Niipp point feature representation on map
class NiippPoint : public Marker
{
	friend class FeaturesFactory;

protected:
	NiippPoint(IObjectsFactory* factory, const QString& id, const QPointF& position);

public:
	virtual ~NiippPoint() {}

	virtual void setPosition(const QPointF &position);
};

} // namespace MapFeature

#endif // NIIPPPOINTFEATURE_H
