#ifndef POLYLINEABSTRACT_H
#define POLYLINEABSTRACT_H

#include <QString>
#include <PwGis/PwGisLonLat.h>
#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/IObjectsFactory.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGis/objects/MapObjectAbstract.h>
#include <PwGis/objects/Path.h>


namespace MapFeature {

class FeaturesFactory;

class PolylineAbstract : public MapObjectAbstract
{
	Q_OBJECT
	friend class FeaturesFactory;

protected:
	IObjectsFactory* m_factory;
	Path* m_path;

	PolylineAbstract( IObjectsFactory* factory, const QString& id,
		const QString& name, PwGisPointList* polyline,
		QObject* parent = 0 );

public:
	virtual ~PolylineAbstract();

	virtual void setPolyline( PwGisPointList* polyline );
	virtual PwGisPointList* polyline();

public slots:
	virtual void updateMap() = 0;
	virtual void removeFromMap() = 0;
};

} // namespace MapFeature

#endif // POLYLINEABSTRACT_H