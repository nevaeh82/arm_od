#include "Map/Features/FeatureAbstract.h"

namespace MapFeature {

FeatureAbstract::FeatureAbstract(IObjectsFactory* factory, const QString& id,
								 const QString& name, const QPointF& position, QObject* parent)
	: MapObjectAbstract( id, name, parent )
	, m_factory( factory )
	, m_position( PwGisLonLat( position.x(), position.y() ) )
{
}

void FeatureAbstract::setPosition(const QPointF& position)
{
	m_position = PwGisLonLat( position.x(), position.y() );
}

QPointF FeatureAbstract::position()
{
	return QPointF( m_position.lon, m_position.lat );
}


} // namespace MapFeature
