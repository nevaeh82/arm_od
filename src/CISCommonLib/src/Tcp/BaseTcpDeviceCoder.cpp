#include "BaseTcpDeviceCoder.h"

BaseTcpDeviceCoder::BaseTcpDeviceCoder(QObject* parent) :
	QObject(parent)
{
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

bool BaseTcpDeviceCoder::isAll()
{
	return true;
}
