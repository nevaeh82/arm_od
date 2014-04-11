#include "Map/Features/StationFeature.h"

namespace MapFeature {

Station::Station( PwGisWidget* pwwidget, QString layerId ) :
	m_pwwidget( pwwidget )
{
	//0 - stations
	PwGisStyle* style = m_pwwidget->createStyle( "stations" );

	style->apply();
}

Station::~Station()
{
}

int Station::readFromFile( QString fileName )
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

} // namespace MapFeature
