#include "Map/IMapStyleManager.h"
#include "Map/Features/AisFeature.h"

namespace MapFeature {

Ais::Ais(IObjectsFactory* factory , const QString& id, const QString& name,
		 const QPointF& position, double course)
	: Marker( factory, id, name, position )
{
	m_marker->addStyleByName( MAP_STYLE_NAME_AIS );

	setCourse( course );
}

void Ais::setCourse(double course)
{
	m_marker->setRotate( course );
	m_marker->setToolTip( QString::number( course ) );
}

} // namespace MapFeature
