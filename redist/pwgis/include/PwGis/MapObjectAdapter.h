#ifndef MAPOBJECTAdapter_H
#define MAPOBJECTAdapter_H
#include <QObject>
#include "objects/IMapObject.h"
#include "objects/IMapObjectInfo.h"
#include "PwGisClient.h"
#include "objects/MapObjectAbstract.h"

/**
 * This class provider unified access to MapObjects for COM.
 * @brief The MapObjectClass class
 */
class GISCLIENT_EXPORT MapObjectAdapter : public QObject, public IMapObject, IMapObjectInfo
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{ca6e2511-57cd-4bcb-bc46-cf55d3f30038}")
	Q_CLASSINFO("InterfaceID", "{b22807e9-9b40-480c-9c7d-b9e258426b18}")

	Q_PROPERTY (QObject* tag READ tag WRITE setTag)
	Q_PROPERTY (QString name READ name WRITE setName)
	Q_PROPERTY (QString id READ id)

private:
	MapObjectAbstract* _owner;
public:
	MapObjectAdapter(QObject* parent=NULL);
	MapObjectAdapter(MapObjectAbstract* owner, QObject* parent);


	virtual void setTag(QObject* tag);
	virtual QObject* tag();

	virtual void setName(QString name);
	virtual QString name();

	virtual QString id() const ;

public slots:
	virtual void updateMap();

	void removeFromMap();

	IMapObject* owner() const;
};

#endif // MAPOBJECTAdapter_H
