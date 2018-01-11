#pragma once

#include <Tcp/BaseTcpClient.h>

class TcpPvoClient : public BaseTcpClient
{
	Q_OBJECT

public:
    explicit TcpPvoClient(QObject* parent = NULL);
    virtual ~TcpPvoClient();

    void sendData(const QByteArray &data);
};
