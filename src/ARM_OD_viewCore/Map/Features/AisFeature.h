#ifndef AISFEATURE_H
#define AISFEATURE_H

#include "Map/Features/MarkerFeature.h"

namespace MapFeature {

class FeaturesFactory;

/// AIS feature in map
class Ais : public Marker
{
	friend class FeaturesFactory;

protected:
	Ais(IObjectsFactory* factory, const QString& id, const QString& name,
		const QPointF& position, double course);

public:
	virtual ~Ais() {}

	void setCourse(double course);
	inline double course() { return m_marker->rotate(); }
};

} // namespace MapFeature

#endif // AISFEATURE_H
