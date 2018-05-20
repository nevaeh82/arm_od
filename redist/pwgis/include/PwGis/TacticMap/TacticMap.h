#ifndef TACTICMAP_H
#define TACTICMAP_H
#include <QObject>
#include <IIdGenerator.h>
#include "TacticMap_global.h"

#include "ITacticMap.h"
#include "ITacticObjectsFactory.h"

class PwGisStyle;
class IMapProvider;
class IStyleFactory;
class ITacticObjectsFactory;
class ITacticStyleFactory;
class MapProvider;
class TacticObjectsFactory;
class TacticStyleFactory;

class TACTICMAP_EXPORT TacticMap: public QObject, public ITacticMap
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{95e4d825-e8a3-4c27-b34f-1ae5a4f35965}")
	Q_CLASSINFO("InterfaceID", "{a384980f-a8da-43a4-b88e-7ef6ead3f0a8}")
	Q_INTERFACES(ITacticMap)

	Q_PROPERTY(ITacticObjectsFactory* objectsFactory READ objectsFactory)
	Q_PROPERTY(ITacticStyleFactory* styleFactory READ styleFactory)

private:
	IMapProvider* _mapProvider;
	IIdGenerator* _idGenerator;
	ITacticStyleFactory* _styleFactory;
	ITacticObjectsFactory* _objectsFactory;

public:
	explicit TacticMap(IMapProvider* mapProvider, IStyleFactory* gisStyleFactory, IIdGenerator* idGenerator, QObject* parent = NULL);

	explicit TacticMap(const TacticMap& another):
		QObject(), ITacticMap(),
		_mapProvider(another._mapProvider),
		_idGenerator(another._idGenerator),
		_styleFactory(another._styleFactory),
		_objectsFactory(another._objectsFactory)
	{
	}

	explicit TacticMap(QObject* parent = 0) :
		QObject(parent),
		_mapProvider(NULL),
		_idGenerator(NULL),
		_styleFactory(NULL),
		_objectsFactory(NULL)
	{
	}
	virtual ~TacticMap();

public slots:
	void setProvider( IMapProvider* provider );
	void setProviderClass( MapProvider* provider );

	virtual void addObject(IMapObject* mapObject);
	virtual void updateObject(IMapObject* mapObject);

	virtual ITacticObjectsFactory* objectsFactory();
	virtual ITacticStyleFactory* styleFactory();

	TacticObjectsFactory* objectsFactoryClass();
	TacticStyleFactory* styleFactoryClass();

	virtual void removeObject(TacticObjectAbstract* object);
};

#endif // TACTICMAP_H
