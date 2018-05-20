#ifndef MAPPROVIDER_H
#define MAPPROVIDER_H
#include <QObject>
#include "IMapProvider.h"
#include "PwGisClient.h"
#include "ObjectsManager.h"
#include "MapObjectEvents.h"
#include <objects/IStyleFactory.h>

class PwGisWidget;
class PwGisStyle;
class IObjectsFactory;
class IIdGenerator;
class IMapObject;
class MapObjectsFactory;
class MapStyleFactory;
class MapObjectAbstract;
class MapObjectAdapter;
class IdGenerator;

namespace Pw { namespace Gis {
class MapManager;
class LayerManager;

class MapBounds;
class IMapBounds;

class ProfileManager;
class IProfileManager;

class IObjectsRegistry;
class ObjectsRegistry;
}}


class GISCLIENT_EXPORT MapProvider: public QObject, public IMapProvider
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{2cd72e6c-816d-4e05-a185-5e3321f099d8}")
	Q_CLASSINFO("InterfaceID", "{cc33b1ae-12d2-43d8-a219-620315341351}")

private:
	PwGisWidget* _gis;
	IIdGenerator* _idGenerator;
	Pw::Gis::IObjectsRegistry* _objectsRegistry;
	IObjectsFactory* _mapFactory;
	IStyleFactory* _styleFactory;
	Pw::Gis::IMapManager* _mapManager;
	Pw::Gis::IMapBounds* _mapBounds;
	Pw::Gis::IProfileManager* _profileManager;
	Pw::Gis::ILayerManager* _layerManager;
	Pw::Gis::IObjectsManager* _objectsManager;

public:
	explicit MapProvider(QObject* parent);
	explicit MapProvider(PwGisWidget* gis, IIdGenerator* idGenerator,  QObject* parent);
	~MapProvider();

public slots:

	/**
	 * @brief Gets mapFactory interface.
	 * @return map objects factory.
	 */
	virtual IObjectsFactory* objectsFactory();

	/**
	 * @brief Gets Style Factory.
	 * @return style factory instance.
	 */
	virtual IStyleFactory* styleFactory();

	/**
	 * @brief Gets id generator strategy.
	 * @return Instance of ID generation stategy.
	 */
	virtual IIdGenerator* idGenerator();

	/**
	 * @brief Execute script on map.
	 * @param JavaScript code.
	 * @return result from WebKit
	 */
	virtual QVariant executeScript(const QString& script);

	/**
	 * @brief Gets mapManager interface.
	 */
	virtual Pw::Gis::IMapManager* mapManager();

	/**
	 * @brief Gets mapBounds interface.
	 */
	virtual Pw::Gis::IMapBounds* mapBounds();

	/**
	* @brief Gets mapBounds interface.
	*/
	virtual Pw::Gis::IObjectsManager* objectsManager();

	/// Gets profile manager.
	/// @return profile manager instance.
	virtual Pw::Gis::IProfileManager* profileManager();

	/// Gets layer manager.
	/// @return layer manager.
	virtual Pw::Gis::ILayerManager* layerManager();

	/// Gets objects registry
	/// @return objects registry interface
	virtual Pw::Gis::IObjectsRegistry* objectsRegistry();

public slots:


	/**
	 * @brief Gets real map factory. required for COM.
	 * @return Map objects factory.
	 */
	MapObjectsFactory*  objectsFactoryClass();

	/**
	 * @brief Gets real style factory. required for COM.
	 * @return Style objects factory.
	 */
	MapStyleFactory* styleFactoryClass();

	/**
	 * @brief Gets  IdGenerator class instance.
	 * @return instance of IDGenerator class.
	 */
	IdGenerator* idGeneratorClass();

	/**
	 * @brief Gets real map bounds. required for COM.
	 * @return instance of MapBounds class.
	 */
	Pw::Gis::MapBounds*  mapBoundsClass();

	/**
	 * @brief Gets real map manager. required for COM.
	 * @return instance of MapManager class.
	 */
	Pw::Gis::MapManager*  mapManagerClass();

	/**
	 * @brief Gets a real profile manager class intance for COM.
	 * @return profile manager instance.
	 */
	Pw::Gis::ProfileManager* profileManagerClass();

	/**
	 * @brief Gets a real layer manager class intance for COM.
	 * @return layer manager instance.
	 */
	Pw::Gis::LayerManager* layerManagerClass();

	/**
	 * @brief Gets a real objects registry class intance for COM.
	 * @return objects registry instance.
	 */
	Pw::Gis::ObjectsRegistry* objectsRegistryClass();

	/**
	 * @brief Gets a real objects registry class intance for COM.
	 * @return objects registry instance.
	 */
	Pw::Gis::ObjectsManager* objectsManagerClass();

};

#endif // MAPPROVIDER_H
