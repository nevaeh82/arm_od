#include "Map/IMapStyleManager.h"
#include "Map/Features/HyperboleFeature.h"

namespace MapFeature {

HyperboleFeature::HyperboleFeature( IObjectsFactory* factory, const QString& id,
	const QString& name )
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


