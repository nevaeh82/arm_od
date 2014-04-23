#include "Map/IMapStyleManager.h"
#include "Map/Features/FriendBplaFeature.h"

namespace MapFeature {

FriendBpla::FriendBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav)
	: BplaAbstract( factory, id, uav )
{
	registerStyle();
	setName( QString::number( uav.uavId ) );
}

void FriendBpla::setName(const QString& name)
{
	int id = name.toInt();
	if( id <= 0 ) {
		Marker::setName( name );
		return;
	}

	QString newName;

	if( id == 1044 ) {
		newName = m_isHistorical ? QObject::tr( "UAV-C-H" ) : QObject::tr( "UAV-C" );
	} else {
		newName = QObject::tr( "UAV (#%1)" )
				.arg( m_isHistorical ? QObject::tr( "%1-H" ).arg( name ) : name );
	}

	Marker::setName( newName );
}

} // namespace MapFeature
