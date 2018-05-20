#ifndef OBJECT_H
#define OBJECT_H

#include <QDataStream>
#include <QList>

#include "BaseItem.h"

typedef struct Object : public BaseItem
{
	Object() : BaseItem()
	{
		type = TypeObject;
	}


}Object;

typedef QList<Object> ObjectsList;

inline QDataStream& operator<<(QDataStream& out, const Object &object)
{
	return out << (quint32)object.id << (quint32)object.pid << object.name << (quint32)object.state << object.isEditable;
}

inline QDataStream& operator>>(QDataStream& in, Object &object)
{
	quint32 inId;
	quint32 inPid;
	quint32 inState;

	in >> inId >> inPid >> object.name >> inState >> object.isEditable;
	object.id = (int)inId;
	object.pid = (int)inPid;
	object.state = (int)inState;

	return in;
}

inline QByteArray objectToByteArray(const Object& obj)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << obj;

	return outBA;
}

inline Object objectFromByteArray(QByteArray& inBA)
{
	Object obj;

	QDataStream stream(inBA);
	stream >> obj;

	return obj;
}


inline QByteArray objectsListToByteArray(const ObjectsList& objList)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << objList;

	return outBA;
}

inline ObjectsList objectsListFromByteArray(QByteArray& inBA)
{
	ObjectsList objList;

	QDataStream stream(inBA);
	stream >> objList;

	return objList;
}

#endif // OBJECT_H
