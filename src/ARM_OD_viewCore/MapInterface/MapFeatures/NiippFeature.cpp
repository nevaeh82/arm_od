#include "NiippFeature.h"


NiippFeature::NiippFeature( PwGisWidget* pwwidget, QString niippLayerId,
	QString niippPointLayerId ) :
	m_pwwidget( pwwidget )
{
	//9 - NIIPPPoint
	PwGisStyle* NIPPPointStyle = m_pwwidget->createStyle( "NIIPPPoint" );
	NIPPPointStyle->setProperty( PwGisStyle::mapFontColor, "brown" );
	NIPPPointStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	NIPPPointStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/Points/images/Points/redmark48.png" );
	NIPPPointStyle->setProperty( PwGisStyle::fillColor, "brown" );
	NIPPPointStyle->setProperty( PwGisStyle::graphicWidth, "20" );
	NIPPPointStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	NIPPPointStyle->setProperty( PwGisStyle::strokeColor, "brown" );
	NIPPPointStyle->setProperty( PwGisStyle::layer, niippPointLayerId );
	NIPPPointStyle->apply();

	//10 - NIIPP
	PwGisStyle* NiiPPStyle = m_pwwidget->createStyle( "NIIPP" );
	NiiPPStyle->setProperty( PwGisStyle::mapFontColor, "brown" );
	NiiPPStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	NiiPPStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/Points/images/Points/redmark48.png" );
	NiiPPStyle->setProperty( PwGisStyle::fillColor, "brown" );
	NiiPPStyle->setProperty( PwGisStyle::graphicWidth, "20" );
	NiiPPStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	NiiPPStyle->setProperty( PwGisStyle::strokeColor, "brown" );
	NiiPPStyle->setProperty( PwGisStyle::layer, niippLayerId );
	NiiPPStyle->apply();
}

NiippFeature::~NiippFeature()
{
}

//add NIIPP-slice
//radius - in projection EPSG:900913 is pseudo meters
//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
void NiippFeature::updateSector( int id, double radius, double bis, QByteArray ba )
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
void NiippFeature::updateCicle( int id, double radius, QByteArray ba )
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

void NiippFeature::addPoint( double lon, double lat )
{
	QString caption;
	caption.append( QObject::tr( "Точка увода" ) );
	caption.append( "\\n" );
	caption.append( QString::number( lat, 'f', 4 ) );
	caption.append( "\\n" );
	caption.append( QString::number( lon, 'f', 4 ) );
	m_pwwidget->addMarker( "NIIPMarker", lon, lat, caption, "", 0, "NIIPPPoint" );
}

