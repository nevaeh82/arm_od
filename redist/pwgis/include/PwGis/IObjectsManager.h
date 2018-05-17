#ifndef IOBJECTSMANAGER_H
#define IOBJECTSMANAGER_H
#include <QObject>

namespace Pw
{
namespace Gis
{

class MapObjectEvents;

class IObjectsManager
{
public:
	virtual ~IObjectsManager(){}

	/**
	* Returns an event handler object
	* @return QObject reference
	*/
	virtual MapObjectEvents&  events() = 0;
};

}
}
#endif // IOBJECTSMANAGER_H

