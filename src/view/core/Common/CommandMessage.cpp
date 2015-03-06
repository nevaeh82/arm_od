#include "CommandMessage.h"

CommandMessage::CommandMessage(int type, QByteArray data)
{
    _type = type;
    _data = data;
}

CommandMessage::~CommandMessage()
{
}

int CommandMessage::get(QByteArray &data)
{
    data = _data;
    return _type;
}

void CommandMessage::clenup()
{
    delete this;
}
