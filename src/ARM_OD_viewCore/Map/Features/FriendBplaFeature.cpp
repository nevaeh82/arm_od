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

	if( m_isHistorical ) {
		newName = id == 1044 ? QObject::tr( "UAV-C-H" ) : QObject::tr( "UAV-H (#%1)" ).arg( id );
	} else {
		newName = id == 1044 ? QObject::tr( "UAV-C" ) : QObject::tr( "UAV (#%1)" ).arg( id );
	}
	Marker::setName( newName );
}

} // namespace MapFeature
