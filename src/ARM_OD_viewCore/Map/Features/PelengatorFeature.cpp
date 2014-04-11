#include "Map/IMapStyleManager.h"
#include "Map/Features/PelengatorFeature.h"

namespace MapFeature {

Pelengator::Pelengator(IObjectsFactory* factory, const QString& id, int postId,
					   const QPointF& position, double angle)
	: FeatureAbstract( factory, id, "", position )
	, m_postId( postId )
	, m_angle( angle )
{
	m_sector = m_factory->createSector();
	m_sector->setOriginPoint( &m_position );
	m_sector->setRadius( 170000 );
	m_sector->addStyleByName( MAP_STYLE_NAME_PELENGATOR );
}

Pelengator::~Pelengator()
{
	m_sector->removeFromMap();
	delete m_sector;
}

void Pelengator::setPosition(const QPointF& position)
{
	FeatureAbstract::setPosition( position );
	m_sector->setOriginPoint( &m_position );
}

void Pelengator::setAngle(double angle)
{
	m_angle = angle;

	double startAngle = m_angle;
	startAngle *= -1;
	startAngle += 90;

	m_sector->setStartAngle( startAngle );
	m_sector->setEndAngle( startAngle + 1 );
}

void Pelengator::update(const QPointF& position, double angle)
{
	bool changed = false;

	if( position != this->position() ) {
		setPosition( position );
		changed = true;
	}

	if( angle != m_angle ) {
		setAngle( angle );
		changed = true;
	}

	if( changed ) {
		updateMap();
	}
}

void Pelengator::updateMap()
{
	m_sector->updateMap();
}

void Pelengator::removeFromMap()
{
	m_sector->removeFromMap();
}

} // namespace MapFeature

