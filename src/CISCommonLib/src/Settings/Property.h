#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QDataStream>
#include <QList>

#include "BaseItem.h"

typedef struct Property : public BaseItem
{
	Property(): BaseItem()
	{
		type = TypeProperty;
	}

	Property(const BaseItem& baseItem)
	{
		id = baseItem.id;
		pid = baseItem.pid;
		name = baseItem.name;
		value = baseItem.value;
		state = baseItem.state;
		isEditable = baseItem.isEditable;

		type = TypeProperty;
	}



}Property;

typedef QList<Property> PropertiesList;


inline QDataStream& operator<<(QDataStream& out, const Property &property)
{
	return out << (quint32)property.id << (quint32)property.pid << property.name << property.value
			   << (quint32)property.state << property.isEditable;
}

inline QDataStream& operator>>(QDataStream& in, Property &property)
{
	quint32 inId;
	quint32 inPid;
	quint32 inState;

	in >> inId >> inPid >> property.name >> property.value >> inState >> property.isEditable;
	property.id = (int)inId;
	property.pid = (int)inPid;
	property.state = (int)inState;

	return in;
}

inline QByteArray propertyToByteArray(const Property& prop)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << prop;

	return outBA;
}

inline Property propertyFromByteArray(QByteArray& inBA)
{
	Property prop;

	QDataStream stream(inBA);
	stream >> prop;

	return prop;
}


inline QByteArray propertiesListToByteArray(const PropertiesList& propList)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << propList;

	return outBA;
}

inline PropertiesList propertiesListFromByteArray(QByteArray& inBA)
{
	PropertiesList propList;

	QDataStream stream(inBA);
	stream >> propList;

	return propList;
}



#endif // PROPERTIES_H
