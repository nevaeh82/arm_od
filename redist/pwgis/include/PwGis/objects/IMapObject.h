#ifndef IMAPOBJECT_H
#define IMAPOBJECT_H
#include <QString>

class QObject;

/// This interface provides any map object functionality
class IMapObject
{
public:
	/// Method performs update of the IMapObject childs on map
	virtual void updateMap() = 0;

	/// Gets object unique identifier.
	virtual QString id() const = 0;

	virtual ~IMapObject() {}
};

#endif // IMAPOBJECT_H
