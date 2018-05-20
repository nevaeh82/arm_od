#ifndef IOBJECTSREGISTRY_H
#define IOBJECTSREGISTRY_H

class QString;
class IMapObject;

#include <QList>

namespace Pw {
namespace Gis {

class Path;
class Marker;
class Circle;
class Rectangle;
class Sector;
class Polygon;

class IObjectsRegistry
{
public:
	virtual ~IObjectsRegistry(){}

	/// add object to registry
	/// @param id objects id
	/// @param obj objects pointer
	virtual void addObject(const QString& id, IMapObject* obj) = 0;

	/// Method returns object pointer by its id. Returns NULL if object not found
	/// @param id objects id
	virtual IMapObject* getObjectById(const QString& id) = 0;

	/// Method shows if object wtih specified id contains in registry
	/// @param id objects id
	virtual bool isObjectExist(const QString& id) = 0;

	/// Method removes object with specified id from registry
	/// @param id objects id
	virtual void removeObject(const QString& id) = 0;

	/// Method returns Path object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Path* getPath(const QString& id) = 0;

	/// Method returns Circle object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Circle* getCircle(const QString& id) = 0;

	/// Method returns Marker object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Marker* getMarker(const QString& id) = 0;

	/// Method returns Polygon object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Polygon* getPolygon(const QString& id) = 0;

	/// Method returns Rectangle object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Rectangle* getRectangle(const QString& id) = 0;

	/// Method returns Sector object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Sector* getSector(const QString& id) = 0;
};

} // Gis
} // Pw

#endif // IOBJECTSREGISTRY_H
