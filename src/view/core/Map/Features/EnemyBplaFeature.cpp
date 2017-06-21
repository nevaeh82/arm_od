#include <QDebug>
#include <QApplication>
#include <QSettings>

#include "Map/Features/EnemyBplaFeature.h"
#include "Map/MapStyleManager.h"

#define TO_KMH 3.6

namespace MapFeature {

EnemyBpla::EnemyBpla(IObjectsFactory* factory, const QString& id, const UavInfo& uav)
	: BplaAbstract( factory, id, uav )
{
	m_styleName = MapStyleManager::getEnemyBplaStyleName( uav.source, uav.statusId );
	m_trackStyleName = MapStyleManager::getEnemyBplaTrackStyleName( uav.source );
	m_errorStyleName = MapStyleManager::getEnemyBplaErrorStyleName( uav.source );

	m_state = uav.statusId;
	m_info = uav;

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
		m_nameTranslated = QObject::tr("_M");
		break;

	case UAV_SOLVER_AUTO_SOURCE:
		m_nameTranslated = QObject::tr("_A");
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

	if(m_isHistorical) {
		m_nameTranslated = m_nameTranslated + QObject::tr("H");
	}

	m_targNameTranslated = QObject::tr("target");
	m_targNameTranslated.prepend("\\n");

	QString path = QApplication::applicationDirPath() + "/Client.ini";
	QSettings settings(path, QSettings::IniFormat);

	m_isExtraInfo = settings.value("Map/mapExtraBoardInfo", 0).toInt();

	if(!m_isExtraInfo) {
		m_originName = ( m_targNameTranslated
					 + uav.name.right(uav.name.length() - uav.name.indexOf("_"))
					 + m_nameTranslated
					 + QObject::tr("\\n %1 Mhz \\n acc %1м").arg(uav.id).arg( QString::number((uav.lonStddev+uav.latStddev)/2, 'f', 1)) /*+
					 + QObject::tr("\\n acc %1м").arg( QString::number((uav.lonStddev+uav.latStddev)/2, 'f', 1))*/
					   );
	} else {
		m_originName = ( m_targNameTranslated
						 + uav.name.right(uav.name.length() - uav.name.indexOf("_"))
						 + m_nameTranslated + QObject::tr("\\n\\n %1 Mhz").arg(uav.id)
						 + QObject::tr("\\n accLon %1м").arg(QString::number(uav.lonStddev, 'f', 1))
						 + QObject::tr("\\n accLat %1м").arg(QString::number(uav.latStddev, 'f', 1)) );
	}



	setName(m_originName);
}

void EnemyBpla::setName(const QString& name)
{
	m_originName = name;

	BplaAbstract::setName(m_originName);
}

void EnemyBpla::update(const UavInfo& uav)
{
	// do not update from uav info with not origin source
	if (m_sourceId != uav.source) return;

	m_info = uav;

	QString tStr = QObject::tr("Target");

	if(uav.statusId != m_state) {
		m_styleName = MapStyleManager::getEnemyBplaStyleName( uav.source, m_state );
		m_marker->removeStyleByName(m_styleName);

		m_state = uav.statusId;
		m_styleName = MapStyleManager::getEnemyBplaStyleName( uav.source, m_state );
		m_marker->addStyleByName(m_styleName);

		registerStyle();
	}

	if(!m_isExtraInfo) {
		m_originName = ( m_targNameTranslated
						 + uav.name.right(uav.name.length() - uav.name.indexOf("_"))
						 + m_nameTranslated + QObject::tr("\\n %1 Mhz").arg(uav.id)
						 + QObject::tr("\\n acc %1м").arg( QString::number((uav.lonStddev+uav.latStddev)/2, 'f', 1))
						);
	} else {
		m_originName = ( m_targNameTranslated
						 + uav.name.right(uav.name.length() - uav.name.indexOf("_"))
						 + m_nameTranslated + QObject::tr("\\n\\n %1 Mhz").arg(uav.id)
						 + QObject::tr("\\n accLon %1м").arg(QString::number(uav.lonStddev, 'f', 1))
						 + QObject::tr("\\n accLat %1м").arg(QString::number(uav.latStddev, 'f', 1))
						);
	}

	setName(m_originName);

	BplaAbstract::update( uav );
	timer->start(lifetime);

}

void EnemyBpla::setExtraInfo(int val)
{
	m_isExtraInfo = val;
}

} // namespace MapFeature

