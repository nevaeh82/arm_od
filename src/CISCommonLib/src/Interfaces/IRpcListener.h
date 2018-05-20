#ifndef IRPCLISTENER_H
#define IRPCLISTENER_H

#include <QObject>

class IRpcListener
{
public:
    virtual ~IRpcListener(){}

	virtual void onMethodCalled(const QString& method, const QVariant& argument) = 0;
};


#endif // IRPCLISTENER_H
