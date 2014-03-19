#ifndef ICLIENT_H
#define ICLIENT_H

#include <QSharedPointer>
class IMessageOld;

class IClient
{
public:
    virtual ~IClient(){};

    virtual void set_id(int id)                                         = 0;
    virtual int get_id()                                                = 0;
    virtual void set_type(int type)                                     = 0;
    virtual int get_type()                                              = 0;
    virtual void send_data(QSharedPointer<IMessageOld> msg_ptr)            = 0;





};

#endif // ICLIENT_H
