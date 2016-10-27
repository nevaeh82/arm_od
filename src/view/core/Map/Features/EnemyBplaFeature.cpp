#include <QDebug>

#include "Map/Features/EnemyBplaFeature.h"
#include "Map/MapStyleManager.h"

#define TO_KMH 3.6

namespace MapFeature {

EnemyBpla::EnemyBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav)
	: BplaAbstract( factory, id, uav )
{
    m_styleName = MapStyleManager::getEnemyBplaStyleName( uav.source, uav.statusId );
	m_trackStyleName = MapStyleManager::getEnemyBplaTrackStyleName( uav.source );

    m_state = uav.statusId;

	if(uav.source == 100 || uav.source == 101 ) {
		lifetime = LIFETIME_1;
	} else {
		lifetime = LIFETIME_2;
	}

	if ( !uav.historical ) {
		m_autoTail = false;
	}

	registerStyle();

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(removeFromMap()));

	// init source label
	switch ( m_sourceId ) {
		case UAV_SOLVER_MANUAL_SOURCE:
            m_nameTranslated = QObject::tr("");
			break;

		case UAV_SOLVER_AUTO_SOURCE:
            m_nameTranslated = QObject::tr("");
			break;

		case UAV_SOLVER_SINGLE_1_SOURCE:
            m_nameTranslated = "";
			m_tailEnabled = false;
			break;

		case UAV_SOLVER_SINGLE_2_SOURCE:
            m_nameTranslated = QObject::tr("");
			m_tailEnabled = false;
			break;

		default:
            m_nameTranslated = QObject::tr("");
			break;
	}

//	m_nameTranslated = m_nameTranslated
//			.arg( m_isHistorical ? QObject::tr( "BPLA_HISTORICAL_SUFFIX" ) : "" )
//			.replace( "%2", "%1" ) + "\\n%2\\n%3\\n%4";

    setName( uav.name );
}

void EnemyBpla::setName(const QString& name)
{
	m_originName = name;

	QString latLonStdDev = QString("(%1, %2)")
			.arg(m_lattitudeStddev, 0, 'f', 2)
			.arg(m_longtitudeStddev, 0, 'f', 2);

//	BplaAbstract::setName( m_nameTranslated.arg(
//							   name,
//							   QString::number( m_altitude, 'f', 1 ),
//							   QString::number( m_speed * TO_KMH, 'f', 1 ),
//							   latLonStdDev ));

    BplaAbstract::setName(m_originName);
}

void EnemyBpla::update(const UavInfo& uav)
{
	// do not update from uav info with not origin source
	if (m_sourceId != uav.source) return;

    if(uav.statusId != m_state) {
        m_styleName = MapStyleManager::getEnemyBplaStyleName( uav.source, m_state );
        m_marker->removeStyleByName(m_styleName);

        m_state = uav.statusId;
        m_styleName = MapStyleManager::getEnemyBplaStyleName( uav.source, m_state );
        m_marker->addStyleByName(m_styleName);

        registerStyle();
    }

	BplaAbstract::update( uav );
	timer->start(lifetime);

}

} // namespace MapFeature

