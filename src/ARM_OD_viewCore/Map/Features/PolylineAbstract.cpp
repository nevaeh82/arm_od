#include "Map/Features/PolylineAbstract.h"


namespace MapFeature {

PolylineAbstract::PolylineAbstract( IObjectsFactory* factory,
	const QString& id, const QString& name,
	PwGisPointList *polyline, QObject* parent )
	: MapObjectAbstract( id, name, parent )
	, m_factory( factory )
{
	m_path = factory->createPath();
	this->setPolyline( polyline );
}

PolylineAbstract::~PolylineAbstract()
{
	m_path->removeFromMap();
	delete m_path;
}

void PolylineAbstract::setPolyline( PwGisPointList* polyline )
{
	PwGisPointList* points = m_path->points();
	points->clear();
	for( int i = 0; i < polyline->count(); ++i ) {
		points->append( new PwGisLonLat( polyline->at(i)->lon,
			polyline->at(i)->lat ) );
	}
}

PwGisPointList* PolylineAbstract::polyline()
{
	return m_path->points();
}

} // namespace MapFeature
