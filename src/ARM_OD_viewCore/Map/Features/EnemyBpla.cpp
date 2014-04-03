#include "EnemyBpla.h"

namespace MapFeature {

EnemyBpla::EnemyBpla( PwGisWidget* pwwidget, QString layerId ) :
	m_pwwidget( pwwidget )
{
	PwGisStyle* BPLAStyle = m_pwwidget->createStyle( "BPLA" );
	BPLAStyle->setProperty( PwGisStyle::mapFontColor, "blue" );
	BPLAStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	BPLAStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/UAV/images/UAV/BPLA48.png" );
	BPLAStyle->setProperty( PwGisStyle::fillColor, "blue" );
	BPLAStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	BPLAStyle->setProperty( PwGisStyle::graphicHeight, "60" );
	BPLAStyle->setProperty( PwGisStyle::strokeColor, "blue" );
	BPLAStyle->setProperty( PwGisStyle::layer, layerId );
	BPLAStyle->apply();
}

EnemyBpla::~EnemyBpla()
{
}

void EnemyBpla::add(int id, QPointF sko, QVector<QPointF> *track,
	double alt )
{
	QString name;
	name = "БПЛА (№" + QString::number( id ) + ")\\n";
	name.append( QString::number( alt, 'f', 1 ) );
	name.append( "\\n" );
	double sko1 = qSqrt( sko.x()*sko.x() + sko.y()*sko.y() );
	name.append( QString::number( sko1, 'f', 4 ) );

//	TODO 2014.03.27 alax
//	PwGisPointList* p = new PwGisPointList();
//	for( int i = 0; i < track.size(); ++i ) {
//		PwGisLonLat *platlon = new PwGisLonLat( track.at(i).y(), track.at(i).x(), this );
//		p->append( platlon );
//	}

	int lastIndex = track->size() - 1;
	m_pwwidget->addMarker( QString::number( id ), track->at( lastIndex ).y(),
		track->at( lastIndex ).x(), name, "", 0, "BPLA" );
}

} // namespace MapFeature

