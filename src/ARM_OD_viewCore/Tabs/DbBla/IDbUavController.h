#ifndef IDBUAVCONTROLLER_H
#define IDBUAVCONTROLLER_H

#include <QString>

struct Uav;
struct UavInfo;
struct UavRole;
struct Devices;
struct UavMission;
struct Target;
struct TargetType;
struct UavType;
struct DeviceType;
struct Status;

struct DBConnectionStruct;

#define INVALID_INDEX -1

class IDbUavController
{
	public:
		virtual ~IDbUavController(){}

		virtual int addUav(const Uav&) = 0;
		virtual int getUavByUavId(const uint uavId) = 0;

		virtual int addUavInfo(const UavInfo&) = 0;
		virtual int getUavInfoByUavId(const uint uavId) = 0;

		virtual int addDevice(const Devices&) = 0;
		virtual bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords) = 0;

		virtual int addUavMission(const UavMission&) = 0;
		virtual bool getUavMissionsByUavId(const uint uavId, QList<UavMission>& missionsRecords) = 0;

		virtual int addTarget(const Target&) = 0;
		virtual bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords) = 0;

		virtual int addTargetType(const TargetType&) = 0;
		virtual int getTargetTypeByName(const QString&) = 0;

		virtual int addUavType(const UavType&) = 0;
		virtual int getUavTypeByName(const QString&) = 0;

		virtual int addDeviceType(const DeviceType&) = 0;
		virtual int getDeviceTypeByName(const QString&) = 0;

		virtual int addStatus(const Status&) = 0;
		virtual int getStatusByName(const QString&) = 0;

		virtual int addUavRole(const UavRole&) = 0;
		virtual int getUavRoleByName(const QString&) = 0;
		virtual int getUavRoleByCode(const QString&) = 0;
};

#endif // IDBUAVCONTROLLER_H