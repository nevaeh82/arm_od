#include "FriendBpla.h"

namespace MapFeature {

FriendBpla::FriendBpla( PwGisWidget* pwwidget,
	QMap< int, PwGisPointList* > *lastCoord, QString layerId ) :
	m_pwwidget( pwwidget ),
	m_lastCoord( lastCoord )
{
	//2 - BLA profile
	PwGisStyle* BLAStyle = m_pwwidget->createStyle( "BLA" );
	BLAStyle->setProperty( PwGisStyle::mapFontColor, "red" );
	BLAStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	BLAStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/UAV/images/UAV/BLA48.png" );
	BLAStyle->setProperty( PwGisStyle::fillColor, "red" );
	BLAStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	BLAStyle->setProperty( PwGisStyle::graphicHeight, "60" );
	BLAStyle->setProperty( PwGisStyle::strokeColor, "red" );
	BLAStyle->setProperty( PwGisStyle::layer, layerId );
	BLAStyle->apply();
}

FriendBpla::~FriendBpla()
{
}

void FriendBpla::add( int id, QPointF point )
{
	PwGisPointList *p;
	if ( m_lastCoord->contains( id ) ) {
		p = m_lastCoord->value( id );
	}
	else {
		p = new PwGisPointList();
		m_lastCoord->insert( id, p );
	}

	QString name;
	if ( id == 1044 ) {
		name  = "БЛА-Ц";
	}
	else {
		name = "БЛА (№" + QString::number( id ) + ")";
	}

	double y = point.y();
	double x = point.x();

//	TODO 2014.03.27 alax
//	PwGisLonLat *platlon = new PwGisLonLat( y, x, this );
	PwGisLonLat *platlon = new PwGisLonLat( y, x );

	p->append(platlon);
	QString path_id = "path_" + QString::number( id );
	if ( p->length() > 100 ) {
		p->removeFirst();
	}

	if ( p->first() == p->last() ) {
		m_pwwidget->removeObject( QString::number( id ) );
		return;
	}

	m_pwwidget->addMarker( QString::number( id ), y, x, name, "", 0, "BLA" );

	m_pwwidget->removeObject( path_id );
	m_pwwidget->addPath( path_id, p, "", "", "BLA" );
}

} // namespace MapFeature
