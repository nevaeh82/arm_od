#ifndef IMESSAGEOLD_H
#define IMESSAGEOLD_H

#include <QByteArray>

class IMessageOld
{
public:
	virtual ~IMessageOld(){};
    virtual QByteArray* get(int& id, int& type) const      = 0;
};

#endif // IMESSAGEOLD_H
