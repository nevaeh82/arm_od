#include "NIIPPParser.h"
#include <QDebug>

NIIPPParser::NIIPPParser()
{
}

NIIPPParser::~NIIPPParser()
{
    qDebug() << "Terminated parser";
}

/// self distroy
void NIIPPParser::distruct()
{
delete this;
}

void NIIPPParser::set_router(IRouter *router)
{
//    _queue = router->get_queue();
}

void NIIPPParser::parsing(QByteArray data)
{
    QString str(data);
    qDebug() << str;
}
