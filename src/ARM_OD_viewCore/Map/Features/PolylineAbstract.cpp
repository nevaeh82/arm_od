#include "Map/Features/PolylineAbstract.h"


namespace MapFeature {

PolylineAbstract::PolylineAbstract( IObjectsFactory* factory,
	const QString& id, const QString& name,
	PwGisPointList *polyline, QObject* parent )
	: MapObjectAbstract( id, name, parent )
	, m_factory( factory )
{
	m_polyline = new PwGisPointList();
	this->setPolyline( polyline );
}

PolylineAbstract::~PolylineAbstract()
{
	m_polyline->clear();
	delete m_polyline;
}

void PolylineAbstract::setPolyline( PwGisPointList* polyline )
{
	m_polyline->clear();
	for( int i = 0; i < polyline->count(); ++i ) {
		m_polyline->append( new PwGisLonLat( polyline->at(i)->lon,
			polyline->at(i)->lat ) );
	}
}

PwGisPointList *PolylineAbstract::polyline()
{
	return m_polyline;
}

} // namespace MapFeature
