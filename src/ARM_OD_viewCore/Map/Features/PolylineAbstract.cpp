#include "Map/Features/PolylineAbstract.h"

namespace MapFeature {

//=========================
//FeatureAbstract::FeatureAbstract(IObjectsFactory* factory,
//   const QString& id,
//	const QString& name,
//    const QPointF& position,
//    QObject* parent)
//	: MapObjectAbstract( id, name, parent )
//	, m_factory( factory )
//	, m_position( PwGisLonLat( position.x(), position.y() ) )
//{
//}

//========================



PolylineAbstract::PolylineAbstract( IObjectsFactory* factory,
	const QString& id, const QString& name,
	const GeoPolyline& polyline, QObject* parent )
	: MapObjectAbstract( id, name, parent )
	, m_factory( factory )
{
	this->setPolyline( polyline );
}

void PolylineAbstract::setPolyline( const GeoPolyline& polyline )
{
	m_polyline.clear();
	for( int i = 0; i < polyline.count(); ++i ) {
		m_polyline.append( PwGisLonLat( polyline.at(i).lon, polyline.at(i).lat ) );
	}
}

GeoPolyline PolylineAbstract::polyline()
{
	return m_polyline;
}

} // namespace MapFeature
