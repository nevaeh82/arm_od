#ifndef DBBLACONTROLLER_H
#define DBBLACONTROLLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include "IDbBlaController.h"

#include "Db/DbControllerBase.h"
#include "Defines.h"
#include "BaseSubject.h"

class DbBlaController : public DbControllerBase, public IDbBlaController
{
	Q_OBJECT
public:
	explicit DbBlaController(QObject *parent = 0);
	virtual ~DbBlaController();

	int addBla(const Bla&);
	int getBlaByBlaId(const uint blaId);

	int addBlaInfo(const BlaInfo&);
	int getBlaInfoByBlaId(const uint blaId);

	int addDevice(const Devices&);
	bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords);

	int addBlaMission(const BlaMission&);
	bool getBlaMissionsByBlaId(const uint blaId, QList<BlaMission>& missionsRecords);

	int addTarget(const Target&);
	bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords);

	int addTargetType(const TargetType&);
	int getTargetTypeByName(const QString&);

	int addBlaType(const BlaType&);
	int getBlaTypeByName(const QString&);

	int addDeviceType(const DeviceType&);
	int getDeviceTypeByName(const QString&);

	int addStatus(const Status&);
	int getStatusByName(const QString&);

private:
	int addDictionaryRecord(const QString& dictionary, const QString& name);
	int getDictionaryRecord(const QString& dictionary, const QString& name);
	
};

#endif // DBBLACONTROLLER_H
