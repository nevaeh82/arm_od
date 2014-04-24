#include <Logger.h>

#include "Map/IMapStyleManager.h"
#include "Map/Features/NiippFeature.h"

namespace MapFeature {

Niipp::Niipp(IObjectsFactory* factory, const QString& id, const ::Niipp& niipp)
	: FeatureAbstract( factory, id, niipp.getName() )
	, m_niippId( niipp.getId() )
	, m_mode( (Mode) niipp.getAntenaType() )
{
	m_circle = factory->createCircle();
	m_circle->addStyleByName( MAP_STYLE_NAME_NIIPP );

	m_sector = factory->createSector();
	m_sector->addStyleByName( MAP_STYLE_NAME_NIIPP );

	setPosition( niipp.getPosition() );
	setRadius( niipp.getRadius() );
	setAngle( niipp.getAngle() );
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
	FeatureAbstract::setPosition( QPointF( position.y(), position.x() ) );

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

void Niipp::setAngle(double value)
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

void Niipp::update(const ::Niipp& niipp)
{
	bool changed = false;

	QPointF position( niipp.getPosition().y(), niipp.getPosition().x() );
	if( position != this->position() ) {
		setPosition( niipp.getPosition() );
		changed = true;
	}

	if( niipp.getAntenaType() != m_mode ) {
		setMode( (Mode) niipp.getAntenaType() );
		changed = true;
	}

	if( niipp.getRadius() != m_radius) {
		setRadius( niipp.getRadius() );
		changed = true;
	}

	if( niipp.getAngle() != m_angle ) {
		setAngle( niipp.getAngle() );
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


