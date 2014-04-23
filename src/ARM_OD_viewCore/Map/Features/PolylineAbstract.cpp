#include "Map/Features/PolylineAbstract.h"

//====================
//alax
#include <QMessageBox>
//====================

namespace MapFeature {

PolylineAbstract::PolylineAbstract( IObjectsFactory* factory,
	const QString& id, const QString& name,
	PwGisPointList *polyline, QObject* parent )
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

void PolylineAbstract::setPolyline( PwGisPointList* polyline )
{
	PwGisPointList* points = m_polyline->points();
	points->clear();

	for( int i = 0; i < polyline->count(); ++i ) {
		points->append( new PwGisLonLat( polyline->at(i)->lon,
			polyline->at(i)->lat ) );
	}


	m_polyline->updateMap();

	QMessageBox msgBox42;
	msgBox42.setText("setPolyline2->" + QString::number( points->count() ) );
	msgBox42.exec();


}

Path* PolylineAbstract::polyline()
{
	return m_polyline;
}

} // namespace MapFeature
