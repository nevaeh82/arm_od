#ifndef TCPSERVERVAENGA_H
#define TCPSERVERVAENGA_H

#include <Tcp/BaseTcpServer.h>

class TcpServerVaenga : public BaseTcpServer
{
    Q_OBJECT

public:
    explicit TcpServerVaenga(QObject* parent = NULL);
    virtual ~TcpServerVaenga();
};

#endif // TCPSERVERVAENGA_H
