#ifndef ITCPSERVER_H
#define ITCPSERVER_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QByteArray>

class ITcpServerClient;

class ITcpServer
{
	public:
		virtual ~ITcpServer(){}

		virtual void start(const QHostAddress& host,  const quint16 port) = 0;
		virtual void stop() = 0;

		virtual bool isListening() = 0;

		virtual bool sendData(const uint clientId, const QByteArray& data) = 0;
		virtual bool sendData(const QByteArray& data) = 0;

		virtual ITcpServerClient* getClientById(uint) = 0;
};

#endif // ITCPSERVER_H
