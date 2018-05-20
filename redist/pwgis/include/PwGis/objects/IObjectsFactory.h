#ifndef IMAPFACTORY_H
#define IMAPFACTORY_H

class PwGisLonLat;

namespace Pw {
namespace Gis {
class Circle;
class Marker;
class Path;
class Rectangle;
class Sector;
class Polygon;
} //Gis
} //Pw

/// This interface provides map objects factory functionality
class IObjectsFactory
{
public:
	virtual ~IObjectsFactory(){}


	/// Method performs Circle map object creation
	/// @return Circle map object pointer
	virtual Pw::Gis::Circle* createCircle() = 0;

	/// Method performs Marker map object creation
	/// @return Marker map object pointer
	virtual Pw::Gis::Marker* createMarker() = 0;

	/// Method performs Polygon map object creation
	/// @return Path map object pointer
	virtual Pw::Gis::Path* createPath() = 0;

	/// Method performs Polygon map object creation
	/// @return Polygon map object pointer
	virtual Pw::Gis::Polygon* createPolygon() = 0;

	/// Method performs Rectangle map object creation
	/// @return Rectangle map object pointer
	virtual Pw::Gis::Rectangle* createRectangle() = 0;

	/// Method performs Sector map object creation
	/// @return Sector map object pointer
	virtual Pw::Gis::Sector* createSector() = 0;

	/**
	 * @brief Create a LonLat point.
	 * @param lon Point longitude.
	 * @param lat Point latitude.
	 * @return Created point.
	 */
	virtual PwGisLonLat* createGeoPoint(double lon, double lat)=0;
};

#endif // IMAPFACTORY_H
