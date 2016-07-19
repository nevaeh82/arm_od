#include "Map/IMapStyleManager.h"
#include "Map/Features/MarkerFeature.h"

namespace MapFeature {

Marker::Marker(IObjectsFactory* factory , const QString& id, const QString& name,
		 const QPointF& position)
	: FeatureAbstract( factory, id, name, position )
	, m_marker( factory->createMarker() )
{
	m_marker->setPosition( &m_position );
	setName( name );
}

Marker::~Marker()
{
	removeFromMap();
	delete m_marker;
}

void Marker::setName(const QString& name)
{
	FeatureAbstract::setName( name );
	m_marker->setName( name );
}

void Marker::setPosition(const QPointF& position)
{
	FeatureAbstract::setPosition( position );
	m_marker->setPosition( &m_position );
}

void Marker::setRotate(const double& angle)
{
	m_marker->setRotate( angle );
}

QString Marker::mapId() const
{
	return m_marker->id();
}

void Marker::updateMap()
{
	m_marker->updateMap();
}

void Marker::removeFromMap()
{
	m_marker->removeFromMap();
}

} // namespace MapFeature
