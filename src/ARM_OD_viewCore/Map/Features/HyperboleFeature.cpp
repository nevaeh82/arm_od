#include "Map/IMapStyleManager.h"
#include "Map/Features/HyperboleFeature.h"

namespace MapFeature {

Hyperbole::Hyperbole( IObjectsFactory* factory,
	const QString& id, const QString& name,
	const Path* polyline, QTime timeMeasure )
	: PolylineAbstract( factory, id, name, polyline )
	, m_timeMeasure( timeMeasure )
{
}

Hyperbole::~Hyperbole()
{
}

void Hyperbole::updateMap()
{
}

void Hyperbole::removeFromMap()
{
}

} // namespace MapFeature


