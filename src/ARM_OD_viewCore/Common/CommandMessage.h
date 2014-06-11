#ifndef COMMANDMESSAGE_H
#define COMMANDMESSAGE_H

#include <QVariant>
#include <QByteArray>

#include "IMessage.h"

#define COMMAND_TOBLACKLIST     201
#define COMMAND_TOWHITELIST     202
#define COMMAND_RECOGNIZESIGNAL 203
#define COMMAND_KM              204

#define COMMAND_PRM_SET_FREQ        301
#define COMMAND_PRM_REQUEST_FREQ    302
#define COMMAND_PRM_SET_ATT1        303
#define COMMAND_PRM_SET_ATT2        304
#define COMMAND_PRM_SET_FILTER      305

///niipp
#define COMMAND_SET_NIIPP_BPLA      401
#define COMMAND_GET_NIIPP_CONNECTION_STATUS     402

/// SOLVER
#define COMMAND_SET_SOLVER          501
#define COMMAND_SET_SOLVER_AUTO     502
#define COMMAND_SET_SOLVER_CLEAR    503



class CommandMessage : public IMessageOld
{
public:
    CommandMessage(int type, QByteArray data);
    ~CommandMessage();

    virtual int get(QByteArray& data);

    virtual void clenup();

private:
    int _type;
    QByteArray _data;
};

#endif // COMMANDMESSAGE_H
