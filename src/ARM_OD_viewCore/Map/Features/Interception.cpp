#include "PerehvatFeature.h"


PerehvatFeature::PerehvatFeature( PwGisWidget* pwwidget, QString layerId ) :
	m_pwwidget( pwwidget )
{
	//7 - Perehvat
	PwGisStyle* perehvatStyle = m_pwwidget->createStyle( "Perehvat" );
	perehvatStyle->setProperty( PwGisStyle::mapFontColor, "red" );
	perehvatStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	perehvatStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/UAV/images/UAV/BPLA48.png" );
	perehvatStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	perehvatStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	perehvatStyle->setProperty( PwGisStyle::strokeColor, "red" );
	perehvatStyle->setProperty( PwGisStyle::layer, layerId );
	perehvatStyle->apply();
}

PerehvatFeature::~PerehvatFeature()
{
}

//radius - in projection EPSG:900913 is pseudo meters
//must use the projection EPSG:28406,28407...; EPSG:32636,32637...
//http://192.168.13.65/pulse/pulse4/index.php?page=task&id=5004&aspect=plan
void PerehvatFeature::addPerehvatPointData( int blaId, int bplaId, QPointF coord,
	float hgt, float radius, float intcCourse, float intcSpeed )
{
	QString pointId = QString::number( blaId ) + "-" + QString::number( bplaId );

	QString caption = QObject::tr( "Высота = " ) + QString::number( hgt ) + "\\n";
	caption.append( QObject::tr( "Радиус = " ) + QString::number( radius ) + "\\n" );
	caption.append( QObject::tr( "Курс = " ) + QString::number( intcCourse ) + "\\n" );
	caption.append( QObject::tr( "Скорость = " ) + QString::number( intcSpeed ) );

	m_pwwidget->addCircle( pointId, coord.y(), coord.x(), radius, caption, "", "Perehvat" );
}
