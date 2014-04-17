#include "Map/IMapStyleManager.h"
#include "Map/Features/BplaAbstractFeature.h"

namespace MapFeature {

BplaAbstract::BplaAbstract(IObjectsFactory* factory, const QString& id, const UavInfo& uav)
	: Marker( factory, id, QString::number( uav.id ), QPointF( uav.lon, uav.lat ) )
	, m_initialized( false )
{
	m_tail = factory->createPath();

	setPosition( position() );
	setAltitude( uav.alt );
	setSpeed( uav.speed );
	setAngle( uav.yaw );

	m_initialized = true;
}

BplaAbstract::~BplaAbstract()
{
	m_tail->removeFromMap();
	delete m_tail;
}

void BplaAbstract::setPosition(const QPointF& position)
{
	Marker::setPosition( position );

	PwGisPointList* points = m_tail->points();

	points->append( new PwGisLonLat( m_position ) );

	while( points->length() > 100 ) {
		PwGisLonLat* point = points->first();
		points->removeFirst();
		delete point;
	}

	if ( points->first() == points->last() ) {
		removeFromMap();
		return;
	}

	updateName();
}

void BplaAbstract::setAltitude(double altitude)
{
	m_altitude = altitude;
	updateName();
}

void BplaAbstract::setSpeed(double speed)
{
	m_speed = speed;
	updateName();
}

void BplaAbstract::setAngle(double angle)
{
	/// \todo: Uncomment, when we going to support BPLA rotation
//	m_marker->setRotate( angle );
	m_angle = angle;
}

void BplaAbstract::update(const UavInfo& uav)
{
	bool changed = false;

	QPointF point( uav.lon, uav.lat );

	if( point != position() ) {
		setPosition( point );
		changed = true;
	}

	if( uav.alt != m_altitude ) {
		m_altitude = uav.alt;
		changed = true;
	}

	if( uav.yaw != m_angle ) {
		m_angle = uav.yaw;
		changed = true;
	}

	if( uav.speed != m_speed ) {
		m_speed = uav.speed;
		changed = true;
	}

	if( changed ) {
		updateName();
	}
}

void BplaAbstract::updateMap()
{
	if( !m_initialized ) return;

	Marker::updateMap();
	m_tail->updateMap();
}

void BplaAbstract::removeFromMap()
{
	m_tail->removeFromMap();
	Marker::removeFromMap();
}

void BplaAbstract::registerStyle()
{
	m_marker->addStyleByName( getStyleName() );
	m_tail->addStyleByName( getStyleName() );
}

} // namespace MapFeature

