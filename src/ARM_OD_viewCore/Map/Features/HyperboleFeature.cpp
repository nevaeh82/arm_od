#include "Map/Features/HyperboleFeature.h"
#include "Map/MapStyleManager.h"


namespace MapFeature {

Hyperbole::Hyperbole( IObjectsFactory* factory,
	IMapStyleManager* styleManager,
	const QString& id, const QString& name,
	PwGisPointList* polyline, const QTime timeMeasure,
	const QString& color )
	: PolylineAbstract( factory, id, name, polyline )
	, m_name( name )
	, m_timeMeasure( timeMeasure )
{
	m_styleManager = styleManager;
	m_path->setName( name );
	QString hyperboleToolTip = timeMeasure.toString( "hh:mm:ss" );
	m_path->setToolTip( hyperboleToolTip );
//=======================

	if ( color.isEmpty() ) {
		m_path->addStyleByName( MAP_STYLE_NAME_HYPERBOLE );
	}
	else {
		PwGisStyle* style = new PwGisStyle( m_styleManager->getHyperboleStyle() );

		style->setProperty( PwGisStyle::strokeColor, "red" );

		m_path->addStyle( style );

	}


	//



	//sector->setStyle(_mapObjectStyle);
	//_mapObjectStyle->line()->setThickness(3); //20 - OLD
	//red color FF0000 hex = 16711680 bin
	//_mapObjectStyle->line()->setColor(16711680); //5413 - OLD
	//=====================
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


