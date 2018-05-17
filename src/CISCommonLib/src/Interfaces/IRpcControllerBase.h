#ifndef IRPCCONTROLLERBASE_H
#define IRPCCONTROLLERBASE_H

#include "IRpcListener.h"
#include "Templates/BaseSubject.h"

class QHostAddress;

class IRpcControllerBase: public BaseSubject<IRpcListener>
{
public:
    virtual ~IRpcControllerBase(){}

	virtual bool start(quint16 port, QHostAddress ipAddress) = 0;
    virtual void stop() = 0;

	/**
	 * Send method to RPC clients
	 * @param method
	 * @param data
	 * @param sender
	 */
	virtual void call(const QString& method, const QVariant& argument, IRpcListener *sender = NULL) = 0;
};

#endif // IRPCCONTROLLERBASE_H
