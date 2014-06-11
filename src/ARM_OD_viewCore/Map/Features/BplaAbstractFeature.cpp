#include "Map/IMapStyleManager.h"
#include "Map/Features/BplaAbstractFeature.h"

#define TRACK_LENGTH 100

namespace MapFeature {

BplaAbstract::BplaAbstract(IObjectsFactory* factory, const QString& id, const UavInfo& uav)
	: Marker( factory, id, QString::number( uav.id ), QPointF( uav.lon, uav.lat ) )
	, m_initialized( false )
	, m_tailEnabled( true )
	, m_autoTail( true )
	, m_lattitudeStddev( 0 )
	, m_longtitudeStddev( 0 )
{
	m_tail = factory->createPath();
	m_slices = factory->createPath();

	setPosition( position() );
	setAltitude( uav.alt );
	setSpeed( uav.speed );
	setAngle( uav.yaw );

	m_isHistorical = uav.historical;
	m_sourceId = uav.source;
	m_initialized = true;
}

BplaAbstract::~BplaAbstract()
{
	m_tail->removeFromMap();
	m_slices->removeFromMap();
	delete m_tail;
	delete m_slices;
}

void BplaAbstract::setPosition(const QPointF& position)
{
	Marker::setPosition( position );

	if ( m_tailEnabled && m_autoTail ) {
		PwGisPointList* points = m_tail->points();

		points->append( new PwGisLonLat( m_position ) );

		while( points->length() > TRACK_LENGTH ) {
			PwGisLonLat* point = points->first();
			points->removeFirst();
			delete point;
		}
	}

	updateName();
}

void BplaAbstract::setTail(const QVector<QPointF>& tail, const QVector<QPointF>& tailStdDev)
{
	Q_UNUSED( tailStdDev );

	PwGisPointList* points = m_tail->points();

	points->clear();

	foreach( QPointF point, tail ) {
		points->append( new PwGisLonLat( point.y(), point.x() ) );
	}

	points->append( new PwGisLonLat( m_position ) );
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

void BplaAbstract::setLattitudeStddev(double lattitude)
{
	m_lattitudeStddev = lattitude;
	updateName();
}

void BplaAbstract::setLongtitudeStddev(double longtitude)
{
	m_longtitudeStddev = longtitude;
	updateName();
}

void BplaAbstract::setSlice(const QPointF& slice)
{
	PwGisPointList* points = m_slices->points();

	points->append( new PwGisLonLat( slice.x(), slice.y() ) );

	while( points->length() > TRACK_LENGTH ) {
		PwGisLonLat* point = points->first();
		points->removeFirst();
		delete point;
	}
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

	if(uav.latStddev != m_lattitudeStddev ) {
		m_lattitudeStddev = uav.latStddev;
		changed = true;
	}

	if(uav.lonStddev != m_longtitudeStddev ) {
		m_longtitudeStddev = uav.lonStddev;
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
	m_slices->updateMap();
}

void BplaAbstract::removeFromMap()
{
	m_tail->removeFromMap();
	m_slices->removeFromMap();
	Marker::removeFromMap();
}

void BplaAbstract::registerStyle()
{
	if( m_isHistorical ) {
		m_marker->addStyleByName( MAP_STYLE_NAME_HISTORY );
		m_tail->addStyleByName( MAP_STYLE_NAME_HISTORY );
		m_slices->addStyleByName( MAP_STYLE_NAME_HISTORY );
	}

	m_marker->addStyleByName( getStyleName() );
	m_tail->addStyleByName( getTrackStyleName() );
	m_slices->addStyleByName( getSliceStyleName() );
}

} // namespace MapFeature

