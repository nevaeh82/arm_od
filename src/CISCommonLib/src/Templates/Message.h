#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QByteArray>
#include "Interfaces/IMessage.h"

template <typename T>
class Message : public QObject, public IMessage<T>
{
private:
	T m_data;
	QString m_type;

public:
	explicit Message(const QString& type, const T& data, QObject* parent = NULL);
	virtual ~Message();
	virtual T data() const;
	virtual QString type() const;
};

template <typename T>
Message<T>::Message(const QString& type, const T& data, QObject* parent) :
	QObject(parent)
{
	m_type = type;
	m_data = data;
}

template <typename T>
Message<T>::~Message()
{
}

template <typename T>
T Message<T>::data() const
{
	return m_data;
}

template <typename T>
QString Message<T>::type() const
{
	return m_type;
}


#endif // MESSAGE_H
