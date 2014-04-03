#include "Map/Features/PelengatorFeature.h"

namespace MapFeature {

Pelengator::Pelengator( PwGisWidget* pwwidget,
	QMap< int, PwGisPointList* > *lastCoord,
	QString pelengatorLayerId, QString pelengatorPointLayerId ) :
	m_pwwidget( pwwidget ),
	m_lastCoord( lastCoord )
{
	//3 - Pelengators
	PwGisStyle* pelengStyle = m_pwwidget->createStyle( "Pelengators" );
	pelengStyle->setProperty( PwGisStyle::mapFontColor, "green" );
	pelengStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	pelengStyle->setProperty( PwGisStyle::fillColor, "green" );
	pelengStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	pelengStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	pelengStyle->setProperty( PwGisStyle::strokeColor, "green" );
	pelengStyle->setProperty( PwGisStyle::layer, pelengatorLayerId );
	pelengStyle->apply();

	//4 - PelengatorsPoint
	PwGisStyle* pelengPointStyle = m_pwwidget->createStyle( "PelengatorsPoint" );
	pelengPointStyle->setProperty( PwGisStyle::mapFontColor, "green" );
	pelengPointStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	pelengPointStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/UAV/images/UAV/BPLA48.png" );
	pelengPointStyle->setProperty( PwGisStyle::fillColor, "green" );
	pelengPointStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	pelengPointStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	pelengPointStyle->setProperty( PwGisStyle::strokeColor, "green" );
	pelengPointStyle->setProperty( PwGisStyle::layer, pelengatorPointLayerId );
	pelengPointStyle->apply();
}

Pelengator::~Pelengator()
{
}

//radius - in projection EPSG:900913 is pseudo meters
//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
void Pelengator::updatePeleng( int postId, double lat, double lon,
	double direction )
{
	QString post_id = QString::number( postId );// + "-" + QString::number(id);

	if ( !m_mapPeleng.contains( post_id ) ) {
		m_mapPeleng.insert( post_id, new PwGisLonLat( lon, lat ) );
	}

	PwGisLonLat* l = m_mapPeleng.value( post_id );

	direction *= -1;
	direction += 90;
	m_pwwidget->addSlice( post_id, l->lon, l->lat, 170000, direction, direction+1, "", "", "Pelengators" );
}

void Pelengator::setPointEvilPeleng( int id, QPointF point )
{
	PwGisPointList* p;
	if ( m_lastCoord->contains( id ) ) {
		p = m_lastCoord->value( id );
	}
	else {
		p = new PwGisPointList();
		m_lastCoord->insert( id, p );
	}
	QString name;
	name = "БПЛА Атлант(№" + QString::number(id) + ")\\n";

	double y = point.y();
	double x = point.x();

//	TODO 2014.03.27 alax
//	PwGisLonLat* platlon = new PwGisLonLat( y, x, this );
	PwGisLonLat* platlon = new PwGisLonLat( y, x );

	p->append( platlon );
	if ( p->length() > 100 ) {
		p->removeFirst();
	}

	QMap<int, int>::iterator it;
	for( it = m_mapPelengPoint.begin(); it != m_mapPelengPoint.end(); ++it ) {
		if ( it.value() != id ) {
			m_pwwidget->removeObject( QString::number( id ) );
			m_mapPelengPoint.remove( id );
		}
	}

	m_pwwidget->addMarker( QString::number( id ), y, x, name, "", 0, "PelengatorsPoint" );
}

} // namespace MapFeature

