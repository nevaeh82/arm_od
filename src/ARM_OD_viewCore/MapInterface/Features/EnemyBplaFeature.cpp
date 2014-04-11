#include <qmath.h>

#include "Map/IMapStyleManager.h"
#include "Map/Features/EnemyBplaFeature.h"

namespace MapFeature {

EnemyBpla::EnemyBpla(IObjectsFactory* factory, const QString& id, int bplaId)
	: Marker( factory, id, "", QPointF() )
{
	setName( QString::number( bplaId ) );

	m_tail = factory->createPath();

	m_tail->addStyleByName( MAP_STYLE_NAME_ENEMY_BPLA );
	m_marker->addStyleByName( MAP_STYLE_NAME_ENEMY_BPLA );
}

EnemyBpla::~EnemyBpla()
{
	m_tail->removeFromMap();
	delete m_tail;
}

void EnemyBpla::setName(const QString& name)
{
	m_originName = name;
	QString newName = "БПЛА (№%1)\\n%2\\n%3";

	double speed = qSqrt( m_speed.x() * m_speed.x() + m_speed.y() * m_speed.y() );

	Marker::setName( newName.arg( name,
								  QString::number( m_altitude, 'f', 1 ),
								  QString::number( speed, 'f', 1 ) ) );
}

void EnemyBpla::setPosition(const QPointF& position)
{
	Marker::setPosition( position );
	updateName();
}

void EnemyBpla::setAltitude(double altitude)
{
	m_altitude = altitude;
	updateName();
}

void EnemyBpla::setTrack(const QVector<QPointF>& track)
{
	m_track = track;

	PwGisPointList* points = m_tail->points();
	points->clear();

	foreach( QPointF point, m_track ) {
		points->append( new PwGisLonLat( point.x(), point.y() ) );
	}

	if( points->count() > 0 ) {
		setPosition( m_track.last() );
	}
}

void EnemyBpla::setSpeed(const QPointF& speed)
{
	m_speed = speed;
	updateName();
}

void EnemyBpla::update(double altitude, const QPointF& speed, const QVector<QPointF>& track)
{
	bool changed = false;

	if( track != m_track ) {
		setTrack( track );
		changed = true;
	}

	if( altitude != m_altitude ) {
		m_altitude = altitude;
		changed = true;
	}

	if( speed != m_speed ) {
		m_speed = speed;
		changed = true;
	}

	if( changed ) {
		updateName();
		updateMap();
	}
}

void EnemyBpla::updateMap()
{
	Marker::updateMap();
	m_tail->updateMap();
}

void EnemyBpla::removeFromMap()
{
	m_tail->removeFromMap();
	Marker::removeFromMap();
}

} // namespace MapFeature

