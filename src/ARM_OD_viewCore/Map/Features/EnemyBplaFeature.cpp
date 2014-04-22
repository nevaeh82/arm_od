#include "Map/Features/EnemyBplaFeature.h"

namespace MapFeature {

EnemyBpla::EnemyBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav)
	: BplaAbstract( factory, id, uav )
{
	registerStyle();
	setName( QString::number( uav.uavId ) );
}

void EnemyBpla::setName(const QString& name)
{
	m_originName = name;
	QString newName = m_isHistorical ? QObject::tr("UAV_enemy-H (#%1)\\n%2\\n%3")
									 : QObject::tr("UAV_enemy (#%1)\\n%2\\n%3");

	BplaAbstract::setName( newName.arg( name,
										QString::number( m_altitude, 'f', 1 ),
										QString::number( m_speed, 'f', 1 ) )
	);
}

} // namespace MapFeature

