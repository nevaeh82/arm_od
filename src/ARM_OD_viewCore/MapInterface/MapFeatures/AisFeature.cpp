#include "AisFeature.h"


AisFeature::AisFeature( PwGisWidget* pwwidget, QString layerId ) :
	m_pwwidget( pwwidget )
{
	//8 - AIS - civil plane
	PwGisStyle* AISStyle = m_pwwidget->createStyle( "AIS" );
	AISStyle->setProperty( PwGisStyle::mapFontColor, "black" );
	AISStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	AISStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/ais/images/AIS/PostPlaneBlack.png" );
	AISStyle->setProperty( PwGisStyle::fillColor, "black" );
	AISStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	AISStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	AISStyle->setProperty( PwGisStyle::strokeColor, "black" );
	AISStyle->setProperty( PwGisStyle::layer, layerId );
	AISStyle->apply();
}

AisFeature::~AisFeature()
{
}

void AisFeature::add( QMap<int, QVector<QString> > aisMap )
{
	this->remove();

	m_mapAis = aisMap;

	QMap<int, QVector<QString> >::iterator it;
	for( it = m_mapAis.begin(); it != m_mapAis.end(); ++it ) {
		QVector<QString> vec = it.value();

		QString id = vec.at(0);
		double lon = vec.at(1).toDouble();
		double lat = vec.at(2).toDouble();
		QString name = vec.at(0);
		QString tooltip;
		tooltip.append(vec.at(5));
		double course = vec.at(5).toDouble();

		m_pwwidget->addMarker( id, lon, lat, name, tooltip, course, "AIS" );
	}
}

void AisFeature::remove()
{
	QMap<int, QVector<QString> >::iterator it;
	for( it = m_mapAis.begin(); it != m_mapAis.end(); ++it ) {
		QVector<QString> vec = it.value();
		QString id = vec.at(0);
		m_pwwidget->removeMarker( id );
	}
	m_mapAis.clear();
}
