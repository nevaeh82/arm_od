#ifndef ITERMINATERPCCONTROLLER_H
#define ITERMINATERPCCONTROLLER_H

#include <QObject>

class ITerminateRpcController
{
public:
	virtual ~ITerminateRpcController(){}

	virtual void sendTerminate(const QString& serviceName) = 0;
};


#endif // ITERMINATERPCCONTROLLER_H
