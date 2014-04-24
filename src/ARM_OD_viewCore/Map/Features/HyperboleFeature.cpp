#include "Map/Features/HyperboleFeature.h"


namespace MapFeature {

Hyperbole::Hyperbole(
	IStyleFactory* styleFactory,
	IObjectsFactory* factory,
	const QString& id,
	const QString& name,
	PwGisPointList* polyline,
	const QTime timeMeasure,
	const QColor color )
	: PolylineAbstract( factory, id, name, polyline )
	, m_styleFactory( styleFactory )
	, m_timeMeasure( timeMeasure )
{

	m_path->addStyleByName( MAP_STYLE_NAME_HYPERBOLE );

	if ( color.isValid() ) {
		PwGisStyle* style = m_styleFactory->createStyle();
		style->setProperty( PwGisStyle::strokeColor, color.name() );
		style->apply();
		m_path->addStyle( style );
	}

	this->updatePath( polyline, timeMeasure );
}

Hyperbole::~Hyperbole()
{
}

void Hyperbole::updatePath( PwGisPointList* polyline, const QTime timeMeasure )
{
	PolylineAbstract::setPolyline( polyline );

	QString hyperboleToolTip = timeMeasure.toString( "hh:mm:ss" );
	m_path->setToolTip( hyperboleToolTip );

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


