#ifndef IQUEUE_H
#define IQUEUE_H

#include <QObject>

class IOrbitor;
class IMessageOld;

class IQueue : public QObject
{
public:
    virtual ~IQueue(){};
    virtual void add_data(IMessageOld *msg)            = 0;
    virtual void set_orbitor(IOrbitor *orbitor)     = 0;
    virtual void terminate(bool state)              = 0;
public slots:
    virtual void exec()                             = 0;
};

#endif // IQUEUE_H
