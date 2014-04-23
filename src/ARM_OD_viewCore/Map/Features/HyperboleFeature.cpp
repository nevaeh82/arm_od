#include "Map/IMapStyleManager.h"
#include "Map/Features/HyperboleFeature.h"


namespace MapFeature {

Hyperbole::Hyperbole(IObjectsFactory* factory,
	const QString& id, const QString& name,
	PwGisPointList* polyline, const QTime timeMeasure )
	: PolylineAbstract( factory, id, name, polyline )
	, m_name( name )
	, m_timeMeasure( timeMeasure )
{
	m_path = factory->createPath();
	//m_path
}

Hyperbole::~Hyperbole()
{
	m_path->removeFromMap();
	delete m_path;
}

void Hyperbole::updatePolyline( PwGisPointList* polyline )
{
	this->setPolyline( polyline );
	this->updateMap();
}

void Hyperbole::updateMap()
{
	m_path->updateMap();
}

void Hyperbole::removeFromMap()
{
	m_path->removeFromMap();
}

} // namespace MapFeature


