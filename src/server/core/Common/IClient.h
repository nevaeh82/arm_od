#ifndef ICLIENT_H
#define ICLIENT_H

#include <QSharedPointer>
class IMessageOld;

class IClient
{
	public:
		virtual ~IClient(){}

		virtual void setId(int id) = 0;
		virtual int getId() = 0;
		virtual void setType(int type) = 0;
		virtual int getType() = 0;
		virtual void sendData(QSharedPointer<IMessageOld> msg_ptr) = 0;
};

#endif // ICLIENT_H
