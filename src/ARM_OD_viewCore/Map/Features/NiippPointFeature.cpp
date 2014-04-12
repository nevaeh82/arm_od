#include "Map/IMapStyleManager.h"
#include "Map/Features/NiippPointFeature.h"

namespace MapFeature {

NiippPoint::NiippPoint(IObjectsFactory* factory, const QString& id,
									 const QPointF& position)
	: Marker( factory, id, "", position )
{
	m_marker->addStyleByName( MAP_STYLE_NAME_NIIPP_POINT );
}

void NiippPoint::setPosition(const QPointF& position)
{
	Marker::setPosition( position );

	QString caption;
	caption.append( QObject::tr("Diversion point") );
	caption.append( "\\n" );
	caption.append( QString::number( m_position.lat, 'f', 4 ) );
	caption.append( "\\n" );
	caption.append( QString::number( m_position.lon, 'f', 4 ) );

	m_marker->setName( caption );
}

} // namespace MapFeature
