#ifndef ITCPPARSER_H
#define ITCPPARSER_H

#include <QByteArray>
class IRouter;

class ITCPParser
{
public:
    virtual ~ITCPParser(){};

    virtual void set_router(IRouter *router)    = 0;
    virtual void parsing(QByteArray data)       = 0;
    virtual void distruct()                     = 0;
};

#endif // ITCPPARSER_H
