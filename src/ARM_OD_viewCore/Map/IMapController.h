#ifndef IMAPCONTROLLER_H
#define IMAPCONTROLLER_H

#include <Interfaces/IController.h>

#include "Map/IMapClient.h"
#include "Map/MapWidget.h"

class IMapController : public IController<MapWidget>
{
public:
	virtual IMapClient* getMapClient( int id ) = 0;
};

#endif // IMAPCONTROLLER_H
