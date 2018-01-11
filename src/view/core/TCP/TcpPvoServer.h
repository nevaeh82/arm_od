#pragma once

#include <Tcp/BaseTcpServer.h>

class TcpPvoServer : public BaseTcpServer
{
	Q_OBJECT

public:
    explicit TcpPvoServer(QObject* parent = NULL);
    virtual ~TcpPvoServer();
};
