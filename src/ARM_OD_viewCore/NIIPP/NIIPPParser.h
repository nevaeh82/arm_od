#ifndef NIIPPPARSER_H
#define NIIPPPARSER_H

#include <QObject>
#include "Interfaces/ITCPParser.h"
#include "Interfaces/core/IRouter.h"
#include <QByteArray>

class NIIPPParser : public QObject, public ITCPParser
{
    Q_OBJECT
public:
    NIIPPParser();
    ~NIIPPParser();

public:
    void distruct();
    void set_router(IRouter *router);
    virtual void parsing(QByteArray data);
};


#endif // NIIPPPARSER_H
