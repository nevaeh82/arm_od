#include "Map/IMapStyleManager.h"
#include "Map/Features/HyperboleFeature.h"

namespace MapFeature {

#include "Map/Features/HyperboleFeature.h"

HyperboleFeature::HyperboleFeature( IObjectsFactory* factory,
	const QString& id, const QString& name,
	const Path* polyline, QTime timeMeasure )
	: PolylineAbstract( factory, id, name, polyline )
	, m_timeMeasure( timeMeasure )
{
}

HyperboleFeature::~HyperboleFeature()
{
}

void HyperboleFeature::updateMap()
{
}

void HyperboleFeature::removeFromMap()
{
}

} // namespace MapFeature


