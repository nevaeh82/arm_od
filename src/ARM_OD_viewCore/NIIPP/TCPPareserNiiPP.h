#ifndef TCPPARESERNIIPP_H
#define TCPPARESERNIIPP_H

#include "Interfaces/ITCPParser.h"
#include "Interfaces/core/IRouter.h"
#include <QByteArray>

class TCPPareserNiiPP: public ITCPParser
{
public:
    TCPPareserNiiPP();
    ~TCPPareserNiiPP();

public:
    virtual void distruct();

    virtual void set_router(IRouter *router);

    virtual void parsing(QByteArray data);
};

#endif // TCPPARESERNIIPP_H
