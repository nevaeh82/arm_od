#include "Map/IMapStyleManager.h"
#include "Map/Features/StationFeature.h"

namespace MapFeature {

Station::Station(IObjectsFactory* factory, const QString& id, const QString& name,
				 const QPointF& position)
    : Marker( factory, id, /*QObject::tr( "PIPS" ) + " " +*/ name, position)
{
	m_marker->addStyleByName( MAP_STYLE_NAME_STATION );
}

} // namespace MapFeature
