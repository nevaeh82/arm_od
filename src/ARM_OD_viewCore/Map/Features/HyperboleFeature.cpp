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
	m_path->setName( name );
	QString hyperboleToolTip = timeMeasure.toString( "hh:mm:ss" );
	m_path->setToolTip( hyperboleToolTip );

	m_path->addStyleByName( MAP_STYLE_NAME_HYPERBOLE );

	this->updatePolyline( polyline );
}

Hyperbole::~Hyperbole()
{
}

void Hyperbole::updatePolyline( PwGisPointList* polyline )
{
	PolylineAbstract::setPolyline( polyline );

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


