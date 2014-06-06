#include "Map/Features/EnemyBplaFeature.h"
#include "Map/MapStyleManager.h"

#define TO_KMH 3.6

namespace MapFeature {

EnemyBpla::EnemyBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav)
	: BplaAbstract( factory, id, uav )
{
	m_styleName = MapStyleManager::getEnemyBplaStyleName( uav.source );
	m_trackStyleName = MapStyleManager::getEnemyBplaTrackStyleName( uav.source );

	registerStyle();

	// init source label
	switch ( m_sourceId ) {
		case UAV_SOLVER_MANUAL_SOURCE:
			m_nameTranslated = QObject::tr("UAV Enemy M");
			break;

		case UAV_SOLVER_AUTO_SOURCE:
			m_nameTranslated = QObject::tr("UAV Enemy A");
			break;

		case UAV_SOLVER_SINGLE_SOURCE:
			m_nameTranslated = QObject::tr("UAV Enemy S");
			break;

		default:
			m_nameTranslated = QObject::tr("UAV Enemy");
			break;
	}

	m_nameTranslated += " " + QObject::tr("(#%1)\\n%2\\n%3\\n%4");

	setName( QString::number( uav.uavId ) );
}

void EnemyBpla::setName(const QString& name)
{
	m_originName = name;

	BplaAbstract::setName( m_nameTranslated.arg(
							   m_isHistorical ? QObject::tr( "%1-H" ).arg( name ) : name,
							   QString::number( m_altitude, 'f', 1 ),
							   QString::number( m_speed * TO_KMH, 'f', 1 ),
							   QString("(%1, %2)")
							   .arg(m_lattitudeStddev, 0, 'f', 10)
							   .arg(m_longtitudeStddev, 0, 'f', 10)) );
}

void EnemyBpla::update(const UavInfo& uav)
{
	// do not update from uav info with not origin source
	if (m_sourceId != uav.source) return;

	BplaAbstract::update( uav );
}

} // namespace MapFeature

