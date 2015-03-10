#ifndef IDBMANAGER_H
#define IDBMANAGER_H

#include <QMap>
#include <QVariant>
#include <QString>

#include <Templates/BaseSubject.h>

#include "Settings/SettingsNode.h"
#include "Interfaces/IDbChangedListener.h"

class IDbManager: public BaseSubject<IDbChangedListener>
{
public:
	virtual ~IDbManager(){}

	virtual SettingsNode getSettingsNode(const QString& objectName) = 0;
	virtual bool updateProperty(const Property& property) = 0;
	virtual bool updatePropertyValue(const QString &objectName, const QString &propName, const QVariant& value) = 0;
	virtual bool updatePropertyForAllObjects(const QString& propName,  const QVariant& value) = 0;

	virtual QString getObjectName(const uint id) const = 0;
	virtual QVariant getPropertyValue(const QString &objectName, const QString &propName) = 0;
};

#endif // IDBMANAGER_H
