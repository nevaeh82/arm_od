#ifndef IMAPCONTROLLER_H
#define IMAPCONTROLLER_H

#include "IMapClient.h"
#include "NIIPP/INiippController.h"
#include "NIIPP/Niipp.h"

class IMapController
{
public:
	virtual ~IMapController() {}
	virtual IMapClient* getMapClient( int id ) = 0;

	virtual void setNiippController(INiippController* controller) = 0;
	virtual void updateNiippPowerZone(const Niipp& niipp) = 0;

	virtual void removeNiippPoint() = 0;
};

#endif // IMAPCONTROLLER_H
