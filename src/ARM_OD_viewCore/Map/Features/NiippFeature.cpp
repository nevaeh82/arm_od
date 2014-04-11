#include "Map/IMapStyleManager.h"
#include "Map/Features/NiippFeature.h"

namespace MapFeature {

Niipp::Niipp(IObjectsFactory* factory, const QString& id, int niippId, const QPointF& position,
			 Niipp::Mode mode, double radius, double angle)
	: FeatureAbstract( factory, id, "", position )
	, m_niippId( niippId )
	, m_mode( mode )
{
	m_circle = factory->createCircle();
	m_circle->setOriginPoint( &m_position );
	m_circle->addStyleByName( MAP_STYLE_NAME_NIIPP );

	m_sector = factory->createSector();
	m_sector->setOriginPoint( &m_position );
	m_sector->addStyleByName( MAP_STYLE_NAME_NIIPP );

	setRadius( radius );
	setAngle( angle );
}

Niipp::~Niipp()
{
	m_circle->removeFromMap();
	delete m_circle;

	m_sector->removeFromMap();
	delete m_sector;
}

void Niipp::setPosition(const QPointF& position)
{
	FeatureAbstract::setPosition( position );

	m_circle->setOriginPoint( &m_position );
	m_sector->setOriginPoint( &m_position );
}

void Niipp::setMode(Niipp::Mode mode)
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

void Niipp::setRadius(double value)
{
	m_radius = value;
	m_circle->setRadius( m_radius );
	m_sector->setRadius( m_radius );
}

void Niipp::setAngle(float value)
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

void Niipp::update(const QPointF& position, Niipp::Mode mode, double radius, double angle)
{
	bool changed = false;

	if( position != this->position() ) {
		setPosition( position );
		changed = true;
	}

	if( mode != m_mode ) {
		setMode( mode );
		changed = true;
	}

	if( radius != m_radius) {
		setRadius( m_radius );
		changed = true;
	}

	if( angle != m_angle ) {
		setAngle( m_angle );
		changed = m_mode == Directed;
	}

	if( changed ) {
		updateMap();
	}
}

void Niipp::updateMap()
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

void Niipp::removeFromMap()
{
	m_circle->removeFromMap();
	m_sector->removeFromMap();
}

} // namespace MapFeature


