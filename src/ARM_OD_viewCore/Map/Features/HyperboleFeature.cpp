#include "Map/IMapStyleManager.h"
#include "Map/Features/HyperboleFeature.h"

namespace MapFeature {

HyperboleFeature::HyperboleFeature(IObjectsFactory* factory, const QString& id, const ::HyperboleFeature& HyperboleFeature)
	: FeatureAbstract( factory, id, HyperboleFeature.getName(), HyperboleFeature.getPoint() )
	, m_HyperboleFeatureId( HyperboleFeature.getId() )
	, m_mode( (Mode) HyperboleFeature.getAntenaType() )
{
	m_circle = factory->createCircle();
	m_circle->setOriginPoint( &m_position );
	m_circle->addStyleByName( MAP_STYLE_NAME_HyperboleFeature );

	m_sector = factory->createSector();
	m_sector->setOriginPoint( &m_position );
	m_sector->addStyleByName( MAP_STYLE_NAME_HyperboleFeature );

	setRadius( HyperboleFeature.getRadius() );
	setAngle( HyperboleFeature.getAngle() );
}

HyperboleFeature::~HyperboleFeature()
{
	m_circle->removeFromMap();
	delete m_circle;

	m_sector->removeFromMap();
	delete m_sector;
}

void HyperboleFeature::setPosition(const QPointF& position)
{
	FeatureAbstract::setPosition( position );

	m_circle->setOriginPoint( &m_position );
	m_sector->setOriginPoint( &m_position );
}

void HyperboleFeature::setMode(HyperboleFeature::Mode mode)
{
	m_mode = mode;

	switch( m_mode ) {
		case Directed:
			m_circle->removeFromMap();
			break;

		case NotDirected:
			m_sector->removeFromMap();
			break;
	}
}

void HyperboleFeature::setRadius(double value)
{
	m_radius = value;
	m_circle->setRadius( m_radius );
	m_sector->setRadius( m_radius );
}

void HyperboleFeature::setAngle(double value)
{
	m_angle = value;

	double startAngle = value + 12.5;

	if ( startAngle < 0 ) {
		startAngle = 360 + startAngle;
	}

	startAngle *= -1;
	startAngle += 90;

	double endAngle = startAngle + 25;

	m_sector->setStartAngle( startAngle );
	m_sector->setEndAngle( endAngle );
}

void HyperboleFeature::update(const ::HyperboleFeature& HyperboleFeature)
{
	bool changed = false;

	if( HyperboleFeature.getPoint() != this->position() ) {
		setPosition( HyperboleFeature.getPoint() );
		changed = true;
	}

	if( HyperboleFeature.getAntenaType() != m_mode ) {
		setMode( (Mode) HyperboleFeature.getAntenaType() );
		changed = true;
	}

	if( HyperboleFeature.getRadius() != m_radius) {
		setRadius( HyperboleFeature.getRadius() );
		changed = true;
	}

	if( HyperboleFeature.getAngle() != m_angle ) {
		setAngle( HyperboleFeature.getAngle() );
		changed = m_mode == Directed;
	}

	if( changed ) {
		updateMap();
	}
}

void HyperboleFeature::updateMap()
{
	switch( m_mode ) {
		case Directed:
			m_sector->updateMap();
			break;

		case NotDirected:
			m_circle->updateMap();
			break;
	}
}

void HyperboleFeature::removeFromMap()
{
	m_circle->removeFromMap();
	m_sector->removeFromMap();
}

} // namespace MapFeature


