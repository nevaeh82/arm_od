#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QObject>
#include <QSqlDatabase>

#include "Settings/Property.h"
#include "Settings/Object.h"
#include "Settings/SettingsNode.h"

#include "Interfaces/IDbController.h"

#define INVALID_INDEX 0

class DbController : public QObject, public IDbController
{
	Q_OBJECT

private:
	QSqlDatabase m_sdb;

public:
	explicit DbController(const QString &dbFile, QObject *parent = NULL);
	virtual ~DbController();

	PropertiesList getProperties(const uint objectID);
	Property getProperty(const uint propID);
	Object getObject(const uint objectId);
	Object getObject(const QString& objectName);

	ObjectsList getAllObjects();

	uint addObject(const Object& object);
	uint addProperty(const Property &property);

	bool setPropertyValue(const uint propId, const QVariant propValue);
	bool setPropertyState(const uint propId, const int state);
	bool setPropertyEditable(const uint propId, const bool isEditable);

private:

	bool createConnection(const QString &dbFile);
	void closeConnection();

	uint addPropToDictionaty(const QString& propName);
	uint isPropInDictionary(const QString& propName);
	QString getPropNameById(const uint propId);

	bool setPropertyParam(const uint propId, const QString& paramName, const QVariant value);

	Object getObjectByParam(const QString& paramName, const QVariant value);

};

#endif // DBCONTROLLER_H
