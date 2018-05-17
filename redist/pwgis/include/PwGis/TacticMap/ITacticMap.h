#ifndef ITACTICMAP_H
#define ITACTICMAP_H
#include <QObject>
#include "TacticMap_global.h"
class IMapObject;
class ITacticObjectsFactory;
class ITacticStyleFactory;
class TacticObjectAbstract;

/// This interface provides Tactic map functionality
class TACTICMAP_EXPORT ITacticMap
{
public:
	virtual ~ITacticMap(){}

	/// Method performs addition of map object to map
	/// @param mapObject Object to add
	virtual void addObject(IMapObject* mapObject) = 0;

	/// Method performs updatuion of map object on map
	/// @param provider MapProvider for map drawing UI access
	virtual void updateObject(IMapObject* mapObject) = 0;

	/// Tactic objects factory getter
	/// @return Tactic object factory
	virtual ITacticObjectsFactory* objectsFactory() = 0;

	/// Map objkects style getter
	/// @return Style factory
	virtual ITacticStyleFactory* styleFactory() = 0;

	/// Removes tacticObject from map
	/// @param object  pointer to the object to be removed
	virtual void removeObject(TacticObjectAbstract* object) = 0;
};
Q_DECLARE_INTERFACE(ITacticMap, "com.pw.gis.ITacticMap/1.0")

#endif // ITACTICMAP_H
