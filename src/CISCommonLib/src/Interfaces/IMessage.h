#ifndef IMESSAGE_H
#define IMESSAGE_H

class QByteArray;
class QString;

#include "Logger/Logger.h"

template <typename T>
class IMessage
{
public:
	virtual ~IMessage(){}
	virtual T data() const = 0;
	virtual QString type() const = 0;
};

#endif // IMESSAGE_H
