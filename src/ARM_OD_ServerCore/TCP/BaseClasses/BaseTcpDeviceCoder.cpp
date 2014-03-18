#include "BaseTcpDeviceCoder.h"

BaseTcpDeviceCoder::BaseTcpDeviceCoder(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;
}

BaseTcpDeviceCoder::~BaseTcpDeviceCoder()
{
}

MessageSP BaseTcpDeviceCoder::encode(const QByteArray& data)
{
	return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, data));
}

QByteArray BaseTcpDeviceCoder::decode(const MessageSP message)
{
	return message->data();
}

QObject* BaseTcpDeviceCoder::asQObject()
{
	return this;
}
