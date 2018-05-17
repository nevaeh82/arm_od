#ifndef BASESUBJECTEX_H
#define BASESUBJECTEX_H

#include "Templates/BaseSubject.h"

template <typename T1, typename T2>
class BaseSubjectEx
{
protected:
	QList<T1*> m_receiversList;
	QList<T2*> m_secondReceiversList;

public:
	virtual ~BaseSubjectEx() {}

	virtual void registerReceiver(T1* receiver);
	virtual void deregisterReceiver(T1* receiver);

	virtual void registerReceiver(T2* receiver);
	virtual void deregisterReceiver(T2* receiver);
};

template <typename T1, typename T2>
void BaseSubjectEx<T1, T2>::registerReceiver(T1* receiver)
{
	m_receiversList.append(receiver);
}

template <typename T1, typename T2>
void BaseSubjectEx<T1, T2>::deregisterReceiver(T1* receiver)
{
	m_receiversList.removeAll(receiver);
}

template <typename T1, typename T2>
void BaseSubjectEx<T1, T2>::registerReceiver(T2* receiver)
{
	m_secondReceiversList.append(receiver);
}

template <typename T1, typename T2>
void BaseSubjectEx<T1, T2>::deregisterReceiver(T2* receiver)
{
	m_secondReceiversList.removeAll(receiver);
}

#endif // BASESUBJECTEX_H
