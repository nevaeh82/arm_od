#ifndef BASERECEIVER_H
#define BASERECEIVER_H

#include <QList>

template <typename T1>
class BaseSubject
{
protected:
	QList<T1*> m_receiversList;

public:
	virtual ~BaseSubject() {}

	virtual void registerReceiver(T1* receiver);
	virtual void deregisterReceiver(T1* receiver);
};

template <typename T1>
void BaseSubject<T1>::registerReceiver(T1* receiver)
{
	m_receiversList.append(receiver);
}

template <typename T1>
void BaseSubject<T1>::deregisterReceiver(T1* receiver)
{
	m_receiversList.removeAll(receiver);
}
#endif // BASERECEIVER_H
