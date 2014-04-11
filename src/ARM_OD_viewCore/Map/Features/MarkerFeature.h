#ifndef MARKERFEATURE_H
#define MARKERFEATURE_H

#include <PwGis/objects/Marker.h>

#include "Map/Features/FeatureAbstract.h"

namespace MapFeature {

/// Represends simple marker without style on the map
class Marker : public FeatureAbstract
{
//	friend class FeaturesFactory;

protected:
	::Marker* m_marker;

	Marker(IObjectsFactory* factory , const QString& id, const QString& name,
			 const QPointF& position);

public:
	virtual ~Marker();

	virtual void setName(const QString& name);

public slots:
	virtual void updateMap();
	virtual void removeFromMap();
};

} // namespace MapFeature

#endif // MARKERFEATURE_H
