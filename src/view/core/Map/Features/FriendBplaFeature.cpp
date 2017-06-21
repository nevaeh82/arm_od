#include "Map/IMapStyleManager.h"
#include "Map/Features/FriendBplaFeature.h"

namespace MapFeature {

FriendBpla::FriendBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav)
	: BplaAbstract( factory, id, uav )
{

	registerStyle();

	m_info = uav;

	setName( uav.name );
}

void FriendBpla::setName(const QString& name)
{
	QString newName;

		newName = QObject::tr( "UAV (#%1)" )
				.arg( m_isHistorical ? QObject::tr( "%1-H" ).arg( name ) : name );

	Marker::setName( newName );
}

} // namespace MapFeature
