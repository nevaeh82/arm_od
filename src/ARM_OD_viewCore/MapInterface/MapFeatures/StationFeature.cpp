#include "StationFeature.h"


StationFeature::StationFeature( PwGisWidget* pwwidget, QString layerId ) :
	m_pwwidget( pwwidget )
{
	//0 - stations
	PwGisStyle* StationStyle = m_pwwidget->createStyle( "stations" );
	StationStyle->setProperty( PwGisStyle::mapFontColor, "red" );
	StationStyle->setProperty( PwGisStyle::mapFontSize, "10pt");
	StationStyle->setProperty( PwGisStyle::externalGraphic, "/profiles/Zav/tabs/images/Punkts/StationRed128.png" );
	StationStyle->setProperty( PwGisStyle::fillColor, "red" );
	StationStyle->setProperty( PwGisStyle::graphicWidth, "40" );
	StationStyle->setProperty( PwGisStyle::graphicHeight, "40" );
	StationStyle->setProperty( PwGisStyle::layer, layerId );
	StationStyle->apply();
}

StationFeature::~StationFeature()
{
}

int StationFeature::readFromFile( QString fileName )
{
	int count = 0;
	QTextCodec* codec = QTextCodec::codecForName( "UTF-8" );
	QSettings stationSettings( fileName, QSettings::IniFormat );

	stationSettings.setIniCodec( codec );

	QString name;
	double lat;
	double lon;

	QStringList childKeys = stationSettings.childGroups();
	foreach( const QString &childKey, childKeys ) {
		stationSettings.beginGroup( childKey );

		name = stationSettings.value( "Name", 0 ).toString();
		lat = stationSettings.value( "Latitude", "0" ).toDouble();
		lon = stationSettings.value( "Longitude", "0" ).toDouble();

		if ( name != QObject::tr( "Ингур" ) ) {
			m_pwwidget->addMarker( name, lon, lat, "ОП " + name, "", 0, "stations" );
		}

		stationSettings.endGroup();
		count++;
	}

	return count;
}
