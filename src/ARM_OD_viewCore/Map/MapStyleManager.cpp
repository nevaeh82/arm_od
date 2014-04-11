#include <Logger.h>

#include "Map/MapStyleManager.h"

#define IMAGE_POST_PLANE_BLACK		"/profiles/Zav/ais/images/AIS/PostPlaneBlack.png"
#define IMAGE_UAV_BPLA_48			"/profiles/Zav/UAV/images/UAV/BLA48.png"

MapStyleManager::MapStyleManager(IStyleFactory* factory)
	: m_factory( factory )
{
}

PwGisStyle* MapStyleManager::createAisStyle(const QString& layerName)
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
		style->setProperty( PwGisStyle::layer, layerName ) ;
		style->apply();
	}

	return style;
}

PwGisStyle*MapStyleManager::createFriendBplaStyle(const QString& layerName)
{
	bool exists;
	PwGisStyle* style = createStyle( MAP_STYLE_NAME_FRIEND_BPLA, &exists );

	if( !exists ) {
		style->setProperty( PwGisStyle::mapFontColor, "red" );
		style->setProperty( PwGisStyle::mapFontSize, "10pt");
		style->setProperty( PwGisStyle::externalGraphic, IMAGE_UAV_BPLA_48 );
		style->setProperty( PwGisStyle::fillColor, "red" );
		style->setProperty( PwGisStyle::graphicWidth, "40" );
		style->setProperty( PwGisStyle::graphicHeight, "60" );
		style->setProperty( PwGisStyle::strokeColor, "red" );
		style->setProperty( PwGisStyle::layer, layerName );
		style->apply();
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
