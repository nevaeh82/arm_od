#include "Map/IMapStyleManager.h"
#include "CheckPointFeature.h"

namespace MapFeature {

CheckPoint::CheckPoint(IObjectsFactory* factory, const QString& id, const QString& name,
								   const QPointF& position)
	: FeatureAbstract( factory, id, name, position )
{
	m_rectangle = factory->createRectangle();
	m_rectangle->addStyleByName( MAP_STYLE_NAME_CHECK_POINTS );

	setName( name );
	setPosition( position );
}

void CheckPoint::setPosition(const QPointF& position)
{
	FeatureAbstract::setPosition( position );

	double stepX = 0.5 / (111.111 * cos ( m_position.lon / (180/3.14) ) );
	double stepY = 0.5 / (111.111);

	PwGisLonLat bottomLeft( m_position.lon - stepX, m_position.lat - stepY );
	PwGisLonLat topRight( m_position.lon + stepX, m_position.lat + stepY );

	m_rectangle->setTopRight( &topRight );
	m_rectangle->setBottomLeft( &bottomLeft );
}

void CheckPoint::updateMap()
{
	m_rectangle->updateMap();
}

void CheckPoint::removeFromMap()
{
	m_rectangle->removeFromMap();
}

CheckPoint::~CheckPoint()
{
	m_rectangle->removeFromMap();
	delete m_rectangle;
}

void CheckPoint::setName(QString name)
{
	FeatureAbstract::setName( name );

	m_rectangle->setName( _name );
}

} // namespace MapFeature
