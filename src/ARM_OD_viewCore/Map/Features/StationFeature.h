#ifndef STATIONFEATURE_H
#define STATIONFEATURE_H

#include "Map/Features/MarkerFeature.h"

namespace MapFeature {

class FeaturesFactory;

/// Station feature representation for map
class Station : public Marker
{
	friend class FeaturesFactory;

protected:
	Station(IObjectsFactory* factory, const QString& id, const QString& name,
			const QPointF& position);

public:
	virtual ~Station() {}
};

} // namespace MapFeature

#endif // STATIONFEATURE_H
