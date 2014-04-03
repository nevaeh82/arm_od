#ifndef ISUBSCRIBER_H
#define ISUBSCRIBER_H

#include <QVector>
#include <QSharedPointer>

#include "Common/IMessage.h"
#include "Common/IClient.h"

//#define RCP_BANDWIDTH_TYPE      101
//#define RCP_SHIFT_TYPE          102
//#define RPC_SS_CORRELATION      103
//#define RCP_COMMAND_RECOGNIZE   104


//#define FLAKON_FFT              1001
//#define FLAKON_CORRELATION      1004
//#define FLAKON_SIGNAL_TYPE      1005

/////PRM
//#define PRM_SET_FREQ            200
//#define PRM_REQUEST_FREQ        201
//#define PRM_SET_ATT1            202
//#define PRM_SET_ATT2            203
//#define PRM_SET_FILTER          204


//#define PRM_STATUS              2000

#include "Defines.h"

class IClient;

class ISubscriber
{
public:
    virtual ~ISubscriber(){};

    virtual void data_ready(int type, QSharedPointer<IMessageOld> msg)         = 0;

    virtual void add_subscription(int type, IClient* client)                = 0;
    virtual void remove_subscription(int type, IClient* client)             = 0;
    virtual void remove_subscription(int type)                              = 0;
    virtual void remove_subscription(IClient* client)                       = 0;

    virtual QVector<IClient *> get_subscribers(int type)                    = 0;

};

#endif // ISUBSCRIBER_H
