#ifndef IMAPPROVIDER_H
#define IMAPPROVIDER_H
#include <QVariant>
#include <QStringList>
#include "IObjectsManager.h"

class IObjectsFactory;
class PwGisPointList;
class PwGisStyle;
class PwGisLonLat;
class IMapObject;
class IStyleFactory;
class IIdGenerator;


namespace Pw { namespace Gis {
class IMapManager;
class IMapBounds;
class IProfileManager;
class ILayerManager;
class IObjectsRegistry;
}}


class IMapProvider
{
public:
	/// Gets map objects factory
	/// @return map objects factory
	virtual IObjectsFactory* objectsFactory()=0;

	/// Gets style provider.
	/// @return style factory.
	virtual IStyleFactory* styleFactory() = 0;

	/// Gets map manager.
	/// @return map manager.
	virtual Pw::Gis::IMapManager* mapManager() = 0;

	/// Gets objects manager.
	/// @return map manager.
	virtual Pw::Gis::IObjectsManager* objectsManager() = 0;

	/// Gets layer manager.
	/// @return layer manager.
	virtual Pw::Gis::ILayerManager* layerManager() = 0;

	/// Gets map bounds.
	/// @return map bounds.
	virtual Pw::Gis::IMapBounds* mapBounds() = 0;

	/// Gets profile manager.
	/// @return profile manager instance.
	virtual Pw::Gis::IProfileManager* profileManager() = 0;

	/// Gets objects registry
	/// @return objects registry interface
	virtual Pw::Gis::IObjectsRegistry* objectsRegistry() = 0;

	/// Execute java script on client
	/// @param script - java script for execution
	/// @return java script execution resul
	virtual QVariant executeScript( const QString& script )=0;

	 /// @brief Gets id generator strategy.
	 /// @return Instance of ID generation stategy.
	virtual IIdGenerator* idGenerator() = 0;

	virtual ~IMapProvider(){}

};

#endif // IMAPPROVIDER_H
