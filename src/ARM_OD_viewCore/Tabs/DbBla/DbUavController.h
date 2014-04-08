#ifndef DbUavController_H
#define DbUavController_H

#include <QObject>
#include <QSqlDatabase>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include "IDbUavController.h"

#include "Db/DbControllerBase.h"
#include "Defines.h"
#include "BaseSubject.h"

class DbUavController : public DbControllerBase, public IDbUavController
{
	Q_OBJECT
public:
	explicit DbUavController(QObject *parent = 0);
	virtual ~DbUavController();

	int addUav(const Uav&);
	Uav getUavByUavId(const uint uavId);
	Uav getUav(const uint id);

	int addUavInfo(const UavInfo&);
	int getUavInfoByUavId(const uint UavId);

	int addDevice(const Devices&);
	bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords);

	int addUavMission(const UavMission&);
	bool getUavMissionsByUavId(const uint uavId, QList<UavMission>& missionsRecords);

	int addTarget(const Target&);
	bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords);

	int addTargetType(const TargetType&);
	int getTargetTypeByName(const QString&);

	int addUavType(const UavType&);
	int getUavTypeByName(const QString&);

	int addDeviceType(const DeviceType&);
	int getDeviceTypeByName(const QString&);

	int addStatus(const Status&);
	int getStatusByName(const QString&);

	int addUavRole(const UavRole&);
	UavRole getUavRole(const uint roleId);
	UavRole getUavRoleByName(const QString&);
	UavRole getUavRoleByCode(const QString&);

private:
	int addDictionaryRecord(const QString& dictionary, const QString& name);
	int getDictionaryRecord(const QString& dictionary, const QString& name);
	
};

#endif // DbUavController_H
