#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QThread>

#include "Interfaces/IDbManager.h"

#include "Interfaces/IDbController.h"
#include "DbController.h"
#include "UiDefines.h"

#include "Interfaces/ISettingsChangedListener.h"


class DbManager : public QObject, public IDbManager
{
	Q_OBJECT

private:
	DbController*       m_dbController;

public:

	DbManager(QObject* parent  = NULL);
	virtual ~DbManager();

	SettingsNode getSettingsNode(const QString& objectName);
	bool updateProperty(const Property& property);
	bool updatePropertyValue(const QString &objectName, const QString &propName, const QVariant& value);
	bool updatePropertyForAllObjects(const QString& propName, const QVariant &value);

	QString getObjectName(const uint id) const;
	QVariant getPropertyValue(const QString& objectName, const QString& propName);

private:
	Object createSettingsNode(const QString& objectName);

	Object createObject(const QString& objectName);
	Object createStation(const QString& stationName);

	Property getProperty(const QString &objectName, const QString &propName);

	void notifyDbChanged(const SettingsNode &category);
	void notifyDbChanged(const Property& property);


signals:
	void signalSetDataDBController(QMap<QString, QVariant> *);
};

#endif // DBMANAGER_H
