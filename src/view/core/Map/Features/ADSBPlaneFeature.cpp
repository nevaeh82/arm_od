#include <QFile>

#include "Map/Features/ADSBPlaneFeature.h"
#include "Map/MapStyleManager.h"

#define TO_KMH 3.6
#define ADSB_TRACK_LENGTH 20

namespace MapFeature {

ADSBPlaneFeature::ADSBPlaneFeature(MapWidget* view, IObjectsFactory* factory, const QString& id, const QPointF& pos)
	: Marker( factory, id, id, pos )
{
	m_styleName = MapStyleManager::getADSBStyleName();
	m_trackStyleName = MapStyleManager::getADSBTrackStyleName();

	lifetime = LIFETIME_ADSB;

	m_tail = factory->createPath();

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(removeFeature()));

	m_nameTranslated = QString("Adsb plane");

	m_marker->addStyleByName( getStyleName() );
	m_tail->addStyleByName( getTrackStyleName() );

	m_popup = new PopupFeature(view, m_marker->id(), pos);
}

ADSBPlaneFeature::~ADSBPlaneFeature()
{

}

void ADSBPlaneFeature::setName(const QString& name)
{
}

void ADSBPlaneFeature::update(const QPointF& pos, const double& yaw)
{
	// do not update from uav info with not origin source
	Marker::setPosition( pos );
	Marker::setRotate( yaw );

	QString html = "<div color=\"white\"> Hello </div>";

	m_popup->update(pos, html);

	PwGisPointList* points = m_tail->points();

	points->append( new PwGisLonLat( pos.x(), pos.y() ) );

	while( points->length() > ADSB_TRACK_LENGTH ) {
		PwGisLonLat* point = points->first();
		points->removeFirst();
		delete point;
	}

	timer->start(lifetime);
}

void ADSBPlaneFeature::onClicked()
{
	m_popup->show();
}

void ADSBPlaneFeature::removeFeature()
{
	Marker::removeFromMap();
	m_tail->removeFromMap();
	m_popup->remove();

	emit onFeatureRemove(id());
}

void ADSBPlaneFeature::updateMap()
{
	Marker::updateMap();
	m_tail->updateMap();
	m_popup->updateMap();
}

QString ADSBPlaneFeature::getMapId() const
{
	return Marker::mapId();
}

} // namespace MapFeature
