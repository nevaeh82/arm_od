#ifndef IMAPCONTROLLER_H
#define IMAPCONTROLLER_H

#include "IMapClient.h"


class IMapController
{
public:
	virtual ~IMapController(){};
	virtual IMapClient  *get_map_client( int id ) = 0;
};

#endif // IMAPCONTROLLER_H
