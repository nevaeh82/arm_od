#include "Map/Features/PolylineAbstract.h"


namespace MapFeature {

PolylineAbstract::PolylineAbstract(IObjectsFactory* factory, const QString& id, const QString& name,
	const QVector<QPointF>& polyline, QObject* parent)
	: MapObjectAbstract( id, name, parent )
	, m_factory( factory )
{
	m_path = m_factory->createPath();
	setPolyline( polyline );
}

PolylineAbstract::~PolylineAbstract()
{
		m_path->removeFromMap();
		delete m_path;
}

void PolylineAbstract::setPolyline(const QVector<QPointF>& polyline)
{
	PwGisPointList* points = m_path->points();
	points->clear();

	foreach( QPointF point, polyline ) {
		points->append( new PwGisLonLat( point.y(), point.x() ) );
	}
}

QVector<QPointF> PolylineAbstract::polyline()
{
	QVector<QPointF> vector;

	for( int i=0; i < m_path->points()->getCount(); i++ ) {
		vector << QPointF( m_path->points()->at(i)->lon, m_path->points()->at(i)->lat );
	}

	return vector;
}

void PolylineAbstract::updateMap()
{
	m_path->updateMap();
}

void PolylineAbstract::removeFromMap()
{
	m_path->removeFromMap();
}

} // namespace MapFeature
