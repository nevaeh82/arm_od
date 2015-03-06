#include "Map/IMapStyleManager.h"
#include "Map/Features/PelengatorPointFeature.h"

namespace MapFeature {

PelengatorPoint::PelengatorPoint(IObjectsFactory* factory, const QString& id, const QString& name,
											 const QPointF& position)
	: Marker( factory, id, name, position )
{
	m_marker->addStyleByName( MAP_STYLE_NAME_PELENGATOR_POINT );
}

void PelengatorPoint::setPosition(const QPointF& position)
{
	Marker::setPosition( position );

	m_track << position;
	if( m_track.size() > 100 ) {
		m_track = m_track.mid( m_track.size() - 100 );
	}
}

} // namespace MapFeature
