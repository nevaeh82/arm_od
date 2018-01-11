#include "TcpPvoClient.h"

TcpPvoClient::TcpPvoClient(QObject* parent) :
	BaseTcpClient(parent)
{
}

TcpPvoClient::~TcpPvoClient()
{
}

void TcpPvoClient::sendData(const QByteArray& data)
{
    writeData(data.data(), data.size());
}
