#ifndef OBJECTSMANAGER_H
#define OBJECTSMANAGER_H
#include "IObjectsManager.h"
#include "MapObjectEvents.h"
#include "PwGisClient.h"

class IObjectsFactory;
class IMapFunctions;
class IStyleFactory;

namespace Pw
{
namespace Gis
{

class IObjectsRegistry;

class GISCLIENT_EXPORT ObjectsManager : public Pw::Gis::MapObjectEvents, public Pw::Gis::IObjectsManager
{
	Q_OBJECT
	Q_ENUMS(FeatureType)
	Q_CLASSINFO("ClassID", "{6b92afff-70b8-4a16-a3eb-b05affa31d16}")
	Q_CLASSINFO("InterfaceID", "{4724c6ad-793e-4a2f-9c19-2db9a85fd2c1}")
	Q_CLASSINFO("EventsID","{e68fa33b-fac8-488b-9b95-b2fd25f133ab}")
	Q_CLASSINFO("CoClassAlias", "ObjectsManager")

public:
	enum FeatureType{
		FeatureTypePoint = 1,
		FeatureTypePath = 2,
		FeatureTypeRectangle = 3,
		FeatureTypeCircle = 4,
		FeatureTypeSlice = 5,
		FeatureTypePolygon = 6,
		FeatureTypeLine = 7,
		FeatureTypeRing = 8,
		FeatureTypeMarker = 9
	};


private:
	IObjectsFactory* _factory;
	IStyleFactory* _styleFactory;
	IObjectsRegistry* _registry;
	IMapFunctions* _map;

	bool updateGeometry(const QString& id, const QString& jsId, const QString& type);

public:
	ObjectsManager(QObject* parent=0);
	ObjectsManager(IMapFunctions* map, IObjectsFactory* factory, IStyleFactory* style, IObjectsRegistry* registry, QObject* parent);

	/**
	* @brief  Map events get property
	* @return Map events object
	*/
	virtual Pw::Gis::MapObjectEvents&  events();

	/**
	* Slot calls by JS when new object added
	* @param id  objects id
	* @param type  objects type
	**/
	virtual void onFeatureAdded(const QString& id, const QString& type);

	/**
	* Slot calls by JS when object modified
	* @param id  objects id
	* @param type  objects type
	*/
	virtual void onFeatureModified(const QString& id, const QString& type);
};

}
}

#endif // OBJECTSMANAGER_H
