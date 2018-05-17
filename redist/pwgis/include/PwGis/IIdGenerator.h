#ifndef IIDGENERATOR_H
#define IIDGENERATOR_H
#include <QString>

class IIdGenerator
{
public:
	virtual QString GetNewID() = 0;
	virtual ~IIdGenerator(){}
};

#endif // IIDGENERATOR_H
