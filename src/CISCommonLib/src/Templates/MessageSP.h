#ifndef MESSAGESP_H
#define MESSAGESP_H

#include <QSharedPointer>

#include "Message.h"

typedef QSharedPointer<IMessage<QByteArray> > MessageSP;

#endif // MESSAGESP_H
