#ifndef POLYLINEABSTRACT_H
#define POLYLINEABSTRACT_H

#include <QString>
#include <QVector>

#include <PwGis/PwGisLonLat.h>
#include <PwGis/objects/IObjectsFactory.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/objects/MapObjectAbstract.h>

typedef QVector<PwGisLonLat> GeoPolyline;

namespace MapFeature {

class FeaturesFactory;

class PolylineAbstract : public MapObjectAbstract
{
	Q_OBJECT
	friend class FeaturesFactory;

protected:
	IObjectsFactory* m_factory;
	GeoPolyline m_polyline;

	PolylineAbstract( IObjectsFactory* factory, const QString& id,
		const QString& name, const GeoPolyline& polyline,
		QObject* parent = 0 );

public:
	virtual ~PolylineAbstract() {}

	virtual void setPolyline( const GeoPolyline& polyline );
	virtual GeoPolyline polyline();

public slots:
	virtual void updateMap() = 0;
	virtual void removeFromMap() = 0;

};

} // namespace MapFeature

#endif // POLYLINEABSTRACT_H
