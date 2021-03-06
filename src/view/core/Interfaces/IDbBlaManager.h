#ifndef IDBBLAMANAGER_H
#define IDBBLAMANAGER_H

#include <QList>
class IDbBlaController;

struct Bla;
struct BlaInfo;
struct Devices;
struct BlaMission;
struct Target;
struct DBConnectionStruct;
struct TargetType;
struct BlaType;
struct DeviceType;
struct Status;

class IDbBlaManager
{
	public:
		virtual ~IDbBlaManager(){}

		virtual void setDbController(IDbBlaController*) = 0;

		virtual int addBla(const Bla&) = 0;
		virtual Bla getBlaByBlaId(const uint blaId) = 0;
		virtual Bla getBla(const uint id) = 0;

		virtual int addBlaInfo(const BlaInfo&) = 0;
		virtual int getBlaInfoByBlaId(const uint blaId) = 0;

		virtual int addDevice(const Devices&) = 0;
		virtual bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords) = 0;

		virtual int addBlaMission(const BlaMission&) = 0;
		virtual bool getBlaMissionsByBlaId(const uint blaId, QList<BlaMission>& missionsRecords) = 0;

		virtual int addTarget(const Target&) = 0;
		virtual bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords) = 0;

		virtual int addTargetType(const TargetType&) = 0;
		virtual int getTargetTypeByName(const QString&) = 0;

		virtual int addBlaType(const BlaType&) = 0;
		virtual int getBlaTypeByName(const QString&) = 0;

		virtual int addDeviceType(const DeviceType&) = 0;
		virtual int getDeviceTypeByName(const QString&) = 0;

		virtual int addStatus(const Status&) = 0;
		virtual int getStatusByName(const QString&) = 0;
};

#endif // IDBBLAMANAGER_H
