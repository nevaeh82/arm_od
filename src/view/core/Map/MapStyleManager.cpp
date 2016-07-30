#include <Logger/Logger.h>

#include "Map/MapStyleManager.h"

#define IMAGE_POST_PLANE_BLACK		"/profiles/Zav/AIS/PostPlaneBlack.png"
#define IMAGE_UAV_BLA_48			"/profiles/Zav/UAV/BLA48.png"
#define IMAGE_UAV_BPLA_48			"/profiles/Zav/UAV/BPLA48.png"
#define IMAGE_ONE_POINT				"/profiles/Zav/UAV/cross.png"
#define IMAGE_STATION_RED_128		"/profiles/Zav/Punkts/StationRed128.png"
#define IMAGE_POINT_RED_48			"/profiles/Zav/Points/redmark48.png"
#define IMAGE_ADSB_PLANE			"/profiles/Zav/AIS/PostPlaneYellow.png"

MapStyleManager::MapStyleManager(IStyleFactory* factory)
	: m_factory( factory )
{
}

PwGisStyle* MapStyleManager::createAisStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_AIS, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "black" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::externalGraphic, IMAGE_POST_PLANE_BLACK );
		style->setProperty( PwGisStyle::fillColor, "black" );
		style->setProperty( PwGisStyle::graphicWidth, "40" );
		style->setProperty( PwGisStyle::graphicHeight, "40" );
		style->setProperty( PwGisStyle::strokeColor, "black" );
		style->setProperty( PwGisStyle::layer, layerId ) ;
	}

	return style;
}

PwGisStyle* MapStyleManager::createFriendBplaStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_FRIEND_BPLA, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "red" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::externalGraphic, IMAGE_UAV_BLA_48 );
		style->setProperty( PwGisStyle::fillColor, "red" );
		style->setProperty( PwGisStyle::graphicWidth, "40" );
		style->setProperty( PwGisStyle::graphicHeight, "60" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createFriendBplaTrackStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_FRIEND_BPLA_TRACK, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::strokeColor, "red" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createFriendBplaSliceStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_FRIEND_BPLA_SLICE, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::strokeColor, "red" );
		style->setProperty( PwGisStyle::strokeOpacity, "0.3" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle*MapStyleManager::createAdsbStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_ADSB, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::strokeColor, "gray" );
		//style->setProperty( PwGisStyle::strokeOpacity, "0.3" );
		style->setProperty( PwGisStyle::layer, layerId );
		style->setProperty( PwGisStyle::externalGraphic, IMAGE_ADSB_PLANE );
		style->setProperty( PwGisStyle::fillColor, "yellow" );
		style->setProperty( PwGisStyle::graphicWidth, "20" );
		style->setProperty( PwGisStyle::graphicHeight, "20" );
	}

	return style;
}

PwGisStyle*MapStyleManager::createAdsbTrackStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_ADSB_TRACK, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::strokeColor, "gray" );
		//style->setProperty( PwGisStyle::strokeOpacity, "0.3" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createEnemyBplaStyle(const QString& layerId, uint source)
{
	bool exists;
	PwGisStyle* style = createStyle( MapStyleManager::getEnemyBplaStyleName(source), &exists );

	if( !exists ) {
		if( source == 100 || source == 101 ) {
			style->setProperty( PwGisStyle::mapFontColor, "blue" );
			style->setProperty( PwGisStyle::mapFontSize, "10pt");
			style->setProperty( PwGisStyle::externalGraphic, IMAGE_UAV_BPLA_48 );
			style->setProperty( PwGisStyle::fillColor, "blue" );
			style->setProperty( PwGisStyle::graphicWidth, "40" );
			style->setProperty( PwGisStyle::graphicHeight, "60" );
			style->setProperty( PwGisStyle::layer, layerId );
		} else {
			style->setProperty( PwGisStyle::mapFontColor, "red" );
			style->setProperty( PwGisStyle::mapFontSize, "5pt");
			style->setProperty( PwGisStyle::externalGraphic, IMAGE_ONE_POINT );
			style->setProperty( PwGisStyle::fillColor, "blue" );
			style->setProperty( PwGisStyle::fillOpacity, "50" );
			style->setProperty( PwGisStyle::graphicWidth, "30" );
			style->setProperty( PwGisStyle::graphicHeight, "30" );
			style->setProperty( PwGisStyle::layer, layerId );
		}
	}

	return style;
}



PwGisStyle* MapStyleManager::createEnemyBplaTrackStyle(const QString& layerId, uint source)
{
	bool exists;
	PwGisStyle* style = createStyle( MapStyleManager::getEnemyBplaTrackStyleName(source), &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::strokeColor, "blue" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createInterceptionStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_INTERCEPTION, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "red" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::externalGraphic, IMAGE_UAV_BPLA_48 );
		style->setProperty( PwGisStyle::graphicWidth, "40" );
		style->setProperty( PwGisStyle::graphicHeight, "40" );
		style->setProperty( PwGisStyle::strokeColor, "red" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createNiippStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_NIIPP, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "brown" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::externalGraphic, IMAGE_POINT_RED_48 );
		style->setProperty( PwGisStyle::fillColor, "brown" );
		style->setProperty( PwGisStyle::graphicWidth, "20" );
		style->setProperty( PwGisStyle::graphicHeight, "40" );
		style->setProperty( PwGisStyle::strokeColor, "brown" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createNiippPointStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_NIIPP_POINT, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "brown" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::externalGraphic, IMAGE_POINT_RED_48 );
		style->setProperty( PwGisStyle::fillColor, "brown" );
		style->setProperty( PwGisStyle::graphicWidth, "20" );
		style->setProperty( PwGisStyle::graphicHeight, "40" );
		style->setProperty( PwGisStyle::strokeColor, "brown" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createPelengatorStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_PELENGATOR, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "green" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::fillColor, "green" );
		style->setProperty( PwGisStyle::graphicWidth, "40" );
		style->setProperty( PwGisStyle::graphicHeight, "40" );
		style->setProperty( PwGisStyle::strokeColor, "green" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createPelengatorPointStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_PELENGATOR_POINT, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "green" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::externalGraphic, IMAGE_UAV_BPLA_48 );
		style->setProperty( PwGisStyle::fillColor, "green" );
		style->setProperty( PwGisStyle::graphicWidth, "40" );
		style->setProperty( PwGisStyle::graphicHeight, "40" );
		style->setProperty( PwGisStyle::strokeColor, "green" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createStationStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_STATION, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "red" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::externalGraphic, IMAGE_STATION_RED_128 );
		style->setProperty( PwGisStyle::fillColor, "red" );
		style->setProperty( PwGisStyle::graphicWidth, "40" );
		style->setProperty( PwGisStyle::graphicHeight, "40" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createGridStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_GRID, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "black" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::fillColor, "black" );
		style->setProperty( PwGisStyle::graphicWidth, "40" );
		style->setProperty( PwGisStyle::graphicHeight, "40" );
		style->setProperty( PwGisStyle::strokeColor, "black" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createCheckPointStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_CHECK_POINTS, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "black" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt" );
		style->setProperty( PwGisStyle::graphicWidth, "40" );
		style->setProperty( PwGisStyle::graphicHeight, "40" );
		style->setProperty( PwGisStyle::strokeColor, "black" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createHistoryStyle(const QString& layerId)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_HISTORY, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createHyperboleStyle( const QString& layerId )
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_HYPERBOLE, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "black" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt" );
		style->setProperty( PwGisStyle::strokeColor, "green" );
        style->setProperty( PwGisStyle::fillColor, "green" );
        style->setProperty( PwGisStyle::fillOpacity, "0.5" );
		style->setProperty( PwGisStyle::layer, layerId );
	}

	return style;
}

PwGisStyle* MapStyleManager::createKKpointStyle(const QString& layerId,
                                                 const double& opacity)
{
    bool exists;

    double val = opacity;
    QString opacitySt = QString::number( opacity, 'g', 1 );
    PwGisStyle* style = createStyle( MAP_STYLE_NAME_POINTKK/*.arg(opacitySt)*/,
                                     &exists );

    if( !exists ) {
        style->setProperty( PwGisStyle::strokeColor, "red" );
        style->setProperty( PwGisStyle::fillColor, "red" );
        style->setProperty( PwGisStyle::fillOpacity, "0.5" );
        style->setProperty( PwGisStyle::strokeOpacity, "0.5" );
        style->setProperty( PwGisStyle::pointRadius, "3" );
        style->setProperty( PwGisStyle::layer, layerId );
    }

    return style;
}

PwGisStyle* MapStyleManager::createWorkAreaStyle(const QString& layerId)
{
    bool exists;
    PwGisStyle* style = createStyle( MAP_STYLE_NAME_WORK_AREA, &exists );

    if( !exists ) {
        style->setProperty( PwGisStyle::strokeColor, "red" );
        style->setProperty( PwGisStyle::fillOpacity, "0" );
        style->setProperty( PwGisStyle::layer, layerId );
    }

    return style;
}

PwGisStyle* MapStyleManager::createStyle(const QString& name, bool* exists)
{
	PwGisStyle* style = getStyle( name, false );
	*exists = false;

	if( style != NULL ) {
		*exists = true;
		log_warning( name + " style already created!" );
	} else {
		style = m_factory->createNamedStyle( name );
		setStyle( name, style );
	}

	return style;
}

PwGisStyle* MapStyleManager::getStyle(const QString& name, bool log)
{
	PwGisStyle* style = m_styles.value( name, NULL );

	if( log && style == NULL ) {
		log_warning( name + " style not created yet!" );
	}

	return style;
}

void MapStyleManager::setStyle(const QString& name, PwGisStyle* style)
{
	if ( name.isEmpty() || style == NULL ) return;

	m_styles.insert( name, style );
}
