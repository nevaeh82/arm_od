#include "Map/Features/NiippFeature.h"

namespace MapFeature {

Niipp::Niipp( PwGisWidget* pwwidget, QString niippLayerId,
	QString niippPointLayerId ) :
	m_pwwidget( pwwidget )
{

}

Niipp::~Niipp()
{
}

//add NIIPP-slice
//radius - in projection EPSG:900913 is pseudo meters
//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
void Niipp::updateSector( int id, double radius, double bis, QByteArray ba )
{
	QString post_id = QString::number( id ) + " - niipp";

	if ( m_mapNiippCircle.contains( post_id ) ) {
		QMap<QString, PwGisLonLat *>::iterator it;
		for( it = m_mapNiippCircle.begin(); it != m_mapNiippCircle.end(); ++it ) {
			QString name = QString::number(id) + " - niipp";
			if ( it.key() == name ) {
				m_pwwidget->removeObject( name );
				m_mapNiippCircle.remove( name );
				break;
			}
		}
	}

	if( !m_mapNiippSector.contains( post_id ) ) {
		QDataStream ds( &ba, QIODevice::ReadOnly );
		QString name;
		ds >> name;
		QPointF latlon;
		ds >> latlon;
		double width_angle;
		ds >> width_angle;
		m_mapNiippSector.insert( post_id, new PwGisLonLat( latlon.y(), latlon.x() ) );
	}
	PwGisLonLat* l = m_mapNiippSector.value( post_id );

	double direction = bis + 12.5;

	if ( direction < 0 ) {
		direction = 360 + direction;
	}

	direction *= -1;
	direction += 90;
	double enddir = direction + 25;
	qDebug() << "AAAAAAAAAAAAAAAAAAA = " << post_id << direction << enddir;
	m_pwwidget->addSlice( post_id, l->lon, l->lat, radius*1000, direction, enddir, "", "", "NIIPP" );
}

//add NIIPP-circle
//radius - in projection EPSG:900913 is pseudo meters
//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
void Niipp::updateCicle( int id, double radius, QByteArray ba )
{
	QString post_id = QString::number( id ) + " - niipp";

	if ( m_mapNiippSector.contains( post_id ) ) {
		QMap<QString, PwGisLonLat *>::iterator it;
		for( it = m_mapNiippSector.begin(); it != m_mapNiippSector.end(); ++it ) {
			QString name = QString::number(id) + " - niipp";
			qDebug() << it.key();
			if ( it.key() == name ) {
				m_pwwidget->removeObject( name );
				m_mapNiippSector.remove( name );
				break;
			}
		}
	}

	if ( !m_mapNiippCircle.contains( post_id ) ) {
		QDataStream ds( &ba, QIODevice::ReadOnly );
		QString name;
		ds >> name;
		QPointF latlon;
		ds >> latlon;
		double width_angle;
		ds >> width_angle;
		m_mapNiippCircle.insert( post_id, new PwGisLonLat( latlon.y(), latlon.x() ) );
	}

	PwGisLonLat* l = m_mapNiippCircle.value( post_id );

	m_pwwidget->addCircle( post_id, l->lon, l->lat, radius*1000, "", "", "NIIPP" );
	m_pwwidget->update();
}

void Niipp::addPoint( double lon, double lat )
{
	QString caption;
	caption.append( QObject::tr( "Точка увода" ) );
	caption.append( "\\n" );
	caption.append( QString::number( lat, 'f', 4 ) );
	caption.append( "\\n" );
	caption.append( QString::number( lon, 'f', 4 ) );
	m_pwwidget->addMarker( "NIIPMarker", lon, lat, caption, "", 0, "NIIPPPoint" );
}

} // namespace MapFeature


