#ifndef DbUavController_H
#define DbUavController_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QSqlDatabase>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include "Tabs/DbBla/IDbUavController.h"
#include "Tabs/DbBla/UavHistory.h"

#include "Db/DbControllerBase.h"
#include "Defines.h"
#include "BaseSubject.h"

class DbUavController : public DbControllerBase, public IDbUavController
{
	friend class testDbUav;

	Q_OBJECT
private:
	QMutex m_addGetUavMutex;
	QMutex m_addGetUavInfoMutex;
	QMutex m_addGetDeviceMutex;
	QMutex m_addGetUavMissionMutex;
	QMutex m_addGetTargetMutex;
	QMutex m_addGetTargetTypeMutex;
	QMutex m_addGetUavTypeMutex;
	QMutex m_addGetDeviceTypeMutex;
	QMutex m_addGetStatusMutex;
	QMutex m_addGetUavRoleMutex;

	QMutex m_addGetDictionaryMutex;

	UavHistory* m_uavHistory;

public:
	explicit DbUavController(QObject *parent = 0);
	explicit DbUavController(QString connectionName, QString dbType, QObject *parent = 0);
	virtual ~DbUavController();

	void disconnectFromDb();

	int addUav(const Uav&);
	Uav getUavByUavId(const uint uavId);
	Uav getUav(const uint id);
	bool getUavsByRole(const QString& role, QList<Uav>& uavs);
	int getUavsCountByRole(const QString& role);

	int addUavInfo(const UavInfo&);
	int getUavInfoByUavId(const uint UavId);

	IUavHistory* getUavHistory();

	int addDevice(const Devices&);
	bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords);

	int addSource(const Source&);
	int getSourceId(const uint sourceId, const uint sourceTypeId);
	bool getSourceByType(const uint sourceTypeId, QList<Source>& sourcesRecords);
	Source getSource(const uint sourceId);

	int addUavMission(const UavMission&);
	bool getUavMissionsByUavId(const uint uavId, QList<UavMission>& missionsRecords);
	bool deleteUavMissionsByUavId(const uint uavId);

	int addTarget(const Target&);
	bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords);
	bool getTargetsByUavId(const uint uavId, QList<Target>& targetsRecords);
	bool getTargetsByMission(const uint missionId, QList<Target> &targetsRecords);
	bool deleteTargetsByUavId(const uint uavId);
	bool deleteTargetsById(const uint id);

	int addTargetType(const TargetType&);
	int getTargetTypeByName(const QString&);

	int addUavType(const UavType&);
	int getUavTypeByName(const QString&);

	int addDeviceType(const DeviceType&);
	int getDeviceTypeByName(const QString&);

	int addSourceType(const SourceType&);
	int getSourceTypeByName(const QString&);

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
