#ifndef MAPOBJECTSFACTORY_H
#define MAPOBJECTSFACTORY_H
#include <QObject>
#include "IObjectsFactory.h"
#include "PwGisClient.h"

class IIdGenerator;
class PwGisLonLat;
class IMapFunctions;

namespace Pw{
namespace Gis{
	class IObjectsRegistry;
	class Circle;
	class Marker;
	class Path;
	class Rectangle;
	class Sector;
	class Polygon;
} //Gis
} //Pw

class GISCLIENT_EXPORT MapObjectsFactory: public QObject, public IObjectsFactory
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{651af81a-be1c-43b6-be06-f5fc8cbde5eb}")
	Q_CLASSINFO("InterfaceID", "{b6348995-7c3f-4101-8215-7e23788efc84}")

private:
	IIdGenerator* _idGenerator;
	IMapFunctions* _map;
	Pw::Gis::IObjectsRegistry* _registry;

public:
	explicit MapObjectsFactory(QObject* parent = NULL);
	explicit MapObjectsFactory(IIdGenerator* idGenerator, IMapFunctions* map, Pw::Gis::IObjectsRegistry* registry, QObject* parent = NULL);

public slots:
	virtual Pw::Gis::Circle* createCircle();
	virtual Pw::Gis::Marker* createMarker();
	virtual Pw::Gis::Path* createPath();
	virtual Pw::Gis::Polygon* createPolygon();
	virtual Pw::Gis::Rectangle* createRectangle();
	virtual Pw::Gis::Sector* createSector();

	/**
	 * @brief Create a LonLat point.
	 * @param lon Point longitude.
	 * @param lat Point latitude.
	 * @return Created point.
	 */
	virtual PwGisLonLat* createGeoPoint(double lon, double lat);
};

#endif // MAPOBJECTSFACTORY_H
