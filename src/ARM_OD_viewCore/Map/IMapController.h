#ifndef IMAPCONTROLLER_H
#define IMAPCONTROLLER_H

#include "IMapClient.h"

class IMapController
{
public:
    virtual ~IMapController(){};
    virtual IMapClient* getMapClient( int id ) = 0;
};

#endif // IMAPCONTROLLER_H
