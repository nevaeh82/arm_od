#ifndef MAPOBJECTABSTRACT_H
#define MAPOBJECTABSTRACT_H
#include <QObject>
#include "IMapObject.h"
#include "IMapObjectInfo.h"
#include "MapObjectStyle.h"

class IMapObject;
class PwGisStyle;
class MapObjectAdapter;
class IMapFunctions;
/**
 * @brief The MapObjectAbstract class holds basic metadata for any object on the map.
 */
class GISCLIENT_EXPORT MapObjectAbstract: public QObject, public IMapObject, public IMapObjectInfo
{
	Q_OBJECT

	Q_PROPERTY (QObject* tag READ tag WRITE setTag)
	Q_PROPERTY (QString name READ name WRITE setName)
	Q_PROPERTY (QString id READ id)

	friend class MapObjectsFactory;

protected:

	QObject* _tag;
	QString _id;
	QString _name;
	MapObjectAdapter* _adapter;
	IMapFunctions* _map;

	explicit MapObjectAbstract(IMapFunctions *map, QString id, QString name, QObject *parent = 0);

public:
	explicit MapObjectAbstract(QString id, QString name, QObject *parent = 0);
	explicit MapObjectAbstract(QObject *parent = 0);

	virtual ~MapObjectAbstract();

	virtual void setTag(QObject* tag);
	virtual QObject* tag();

	virtual void setName(QString name);
	virtual QString name();

	virtual QString id() const ;

public slots:
	virtual void updateMap();

	virtual void removeFromMap();

	MapObjectAdapter* adapter();
};

#endif // MAPOBJECTABSTRACT_H
