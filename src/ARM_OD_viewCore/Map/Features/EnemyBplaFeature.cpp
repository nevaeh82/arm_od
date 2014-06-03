#include "Map/Features/EnemyBplaFeature.h"

#define TO_KMH 3.6

namespace MapFeature {

EnemyBpla::EnemyBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav)
	: BplaAbstract( factory, id, uav )
{
	registerStyle();
	setName( QString::number( uav.uavId ) );
}

void EnemyBpla::setName(const QString& name)
{
	double speedKmh;
	m_originName = name;
	QString newName = QObject::tr("UAV_enemy (#%1)\\n%2\\n%3\\n%4");

	speedKmh = m_speed * TO_KMH;

	BplaAbstract::setName( newName.arg( m_isHistorical ? QObject::tr( "%1-H" ).arg( name ) : name,
										QString::number( m_altitude, 'f', 1 ),
										QString::number( speedKmh, 'f', 1 ),
										"(" + QString::number(m_lattitudeStddev) +
										QString(", ") +
										QString::number(m_longtitudeStddev) + ")" )
	);
}

} // namespace MapFeature

