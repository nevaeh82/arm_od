#ifndef OBJECTSREGISTRY_H
#define OBJECTSREGISTRY_H
#include <QMap>
#include <QObject>
#include "PwGisClient.h"
#include <IObjectsRegistry.h>

class QString;
class IMapObject;


namespace Pw {
namespace Gis {

class Path;
class Marker;
class Circle;
class Rectangle;
class Sector;
class Polygon;

class GISCLIENT_EXPORT ObjectsRegistry : public QObject, public IObjectsRegistry
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{f9fc7d12-5af6-4770-af22-87f1d6b2e838}")
	Q_CLASSINFO("InterfaceID", "{e0dcd36b-d201-4eb4-8939-f611aa157bb2}")
	Q_CLASSINFO("CoClassAlias", "ObjectsRegistry")

	QMap< QString, IMapObject* > _objects;

public:
	ObjectsRegistry(QObject* parent = 0);

public slots:
	/// add object to registry
	/// @param id objects id
	/// @param obj objects pointer
	virtual void addObject(const QString& id, IMapObject* obj);

	/// Method returns object pointer by its id. Returns NULL if object not found
	/// @param id objects id
	virtual IMapObject* getObjectById(const QString& id);

	/// Method shows if object wtih specified id contains in registry
	/// @param id objects id
	virtual bool isObjectExist(const QString& id);

	/// Method removes object with specified id from registry
	/// @param id objects id
	virtual void removeObject(const QString& id);

	/// Method returns Path object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Path* getPath(const QString& id);

	/// Method returns Circle object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Circle* getCircle(const QString& id);

	/// Method returns Marker object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Marker* getMarker(const QString& id);

	/// Method returns Polygon object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Polygon* getPolygon(const QString& id);

	/// Method returns Rectangle object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Rectangle* getRectangle(const QString& id);

	/// Method returns Sector object by its id. Returns NULL if object not found
	/// or object doesn't match a type
	/// @param id objects id
	virtual Pw::Gis::Sector* getSector(const QString& id);
};

} // Gis
} // Pw

#endif // OBJECTSREGISTRY_H
