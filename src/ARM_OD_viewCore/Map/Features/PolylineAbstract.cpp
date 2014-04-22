#include "Map/Features/PolylineAbstract.h"

namespace MapFeature {

PolylineAbstract::PolylineAbstract(IObjectsFactory* factory,
	const QString& id, const QString& name,
	const Path* polyline, QObject* parent )
	: MapObjectAbstract( id, name, parent )
	, m_factory( factory )
{
	m_polyline = factory->createPath();
	this->setPolyline( polyline );
}

//PolylineAbstract::~PolylineAbstract()
//{
//	m_polyline->removeFromMap();
//	delete m_polyline;
//}

void PolylineAbstract::setPolyline( const Path* polyline )
{
	PwGisPointList* points = m_polyline->points();
	points->clear();

	/*
	points->append( new PwGisLonLat( m_position ) );



	polyline->points().
	m_polyline.clear();
	for( int i = 0; i < polyline->.count(); ++i ) {
		m_polyline.append( PwGisLonLat( polyline.at(i).lon, polyline.at(i).lat ) );
	}
	*/
}

Path* PolylineAbstract::polyline()
{
	return m_polyline;
}

} // namespace MapFeature
