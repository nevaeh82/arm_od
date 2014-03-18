#ifndef BASERECEIVER_H
#define BASERECEIVER_H

#include <QList>

template <typename T>
class BaseSubject
{
protected:
	QList<T*> m_receiversList;

public:
	virtual ~BaseSubject() {}

	virtual void registerReceiver(T* receiver);
	virtual void deregisterReceiver(T* receiver);
};

template <typename T>
void BaseSubject<T>::registerReceiver(T* receiver)
{
	m_receiversList.append(receiver);
}

template <typename T>
void BaseSubject<T>::deregisterReceiver(T* receiver)
{
	int index = m_receiversList.indexOf(receiver);

	if (index < 0){
		return;
	}

	m_receiversList.removeAt(index);
}

#endif // BASERECEIVER_H
