#ifndef IDBUAVMANAGER_H
#define IDBUAVMANAGER_H

#include <QList>
#include <QMap>

class IUavHistory;
class IDbUavController;

struct Uav;
struct UavInfo;
struct UavRole;
struct Devices;
struct Source;
struct UavMission;
struct Target;
struct TargetType;
struct UavType;
struct DeviceType;
struct SourceType;
struct Status;

struct DBConnectionStruct;

class IDbUavManager
{
public:
	virtual ~IDbUavManager(){}

	virtual QMap<uint, Uav> getKnownUavList() = 0;
	virtual void setDbController(IDbUavController*) = 0;

	virtual int addUav(const Uav&) = 0;
	virtual Uav getUavByUavId(const uint uavId) = 0;
	virtual Uav getUav(const uint id) = 0;
	virtual bool getUavsByRole(const QString& role, QList<Uav>& uavs) = 0;

	virtual int addUavInfo(const UavInfo&, bool actual = true,
						   const QVector<QPointF>& tail = QVector<QPointF>(),
						   const QVector<QPointF>& tailStdDev = QVector<QPointF>()) = 0;
	virtual bool getUavInfoByUavId(const uint uavId, QList<UavInfo>& uavInfoList) = 0;

	virtual IUavHistory* getUavHistory() = 0;

	virtual int addDevice(const Devices&) = 0;
	virtual bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords) = 0;

	virtual int addSource(const Source&) = 0;
	virtual int getSourceId(const uint sourceId, const uint sourceTypeId) = 0;
	virtual bool getSourcesByType(const uint sourceId, QList<Source>& sourcesRecords) = 0;
	virtual Source getSource(const uint sourceId) = 0;

	virtual int addUavMission(const UavMission&) = 0;
	virtual bool getUavMissionsByUavId(const uint uavId, QList<UavMission>& missionsRecords) = 0;
	virtual bool deleteUavMissionsByUavId(const uint uavId) = 0;
	virtual bool deleteUavMissionsByTargetId(const uint targetId) = 0;

	virtual int addTarget(const Target&) = 0;
	virtual bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords) = 0;
	virtual bool getTargetsByUavId(const uint uavId, QList<Target>& targetsRecords) = 0;
	virtual bool getTargetsByUavIdAndTargetType(const uint uavId, const QString& targetType, QList<Target>& targetsRecords) = 0;
	virtual bool deleteTargetsByUavId(const uint uavId) = 0;
	virtual bool deleteTargetsById(const uint id) = 0;

	virtual int addTargetType(const TargetType&) = 0;
	virtual int getTargetTypeByName(const QString&) = 0;

	virtual int addUavType(const UavType&) = 0;
	virtual int getUavTypeByName(const QString&) = 0;

	virtual int addDeviceType(const DeviceType&) = 0;
	virtual int getDeviceTypeByName(const QString&) = 0;

	virtual int addSourceType(const SourceType&) = 0;
	virtual int getSourceTypeByName(const QString&) = 0;

	virtual int addStatus(const Status&) = 0;
	virtual int getStatusByName(const QString&) = 0;

	virtual int addUavRole(const UavRole&) = 0;
	virtual UavRole getUavRole(const uint) = 0;
	virtual UavRole getUavRoleByName(const QString&) = 0;
	virtual UavRole getUavRoleByCode(const QString&) = 0;

	virtual void deleteAllUav() = 0;
};

#endif // IDBUAVMANAGER_H
