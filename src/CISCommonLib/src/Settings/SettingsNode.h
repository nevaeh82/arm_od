#ifndef SETTINGSNODE_H
#define SETTINGSNODE_H

#include "Object.h"
#include "Property.h"

typedef struct SettingsNode
{
	Object object;
	PropertiesList properties;

}SettingsNode;

typedef QList<SettingsNode> SettingsNodeList;


inline QDataStream& operator<<(QDataStream& out, const SettingsNode &settingsNode)
{
	return out << settingsNode.object << settingsNode.properties;
}

inline QDataStream& operator>>(QDataStream& in, SettingsNode &settingsNode)
{
	in >> settingsNode.object >> settingsNode.properties;
	return in;
}

inline QByteArray settingsNodeToByteArray(const SettingsNode& settingsNode)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << settingsNode;

	return outBA;
}

inline SettingsNode settingsNodeFromByteArray(QByteArray& inBA)
{
	SettingsNode settingsNode;

	QDataStream stream(inBA);
	stream >> settingsNode;

	return settingsNode;
}

inline QByteArray settingsNodeListToByteArray(const SettingsNodeList& settingsNodeList)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << settingsNodeList;

	return outBA;
}

inline SettingsNodeList settingsNodeListFromByteArray(QByteArray& inBA)
{
	SettingsNodeList settingsNodeList;

	QDataStream stream(inBA);
	stream >> settingsNodeList;

	return settingsNodeList;
}


#endif // SETTINGSNODE_H
