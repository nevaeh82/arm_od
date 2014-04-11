#ifndef TESTDB_H
#define TESTDB_H

#include <cxxtest/TestSuite.h>

#include <QTime>

#include "Tabs/DbBla/DbUavController.h"
#include "Tabs/DbBla/Defines.h"
#include "Db/Defines.h"
#include "Interfaces/IDbControllerBase.h"

#include "Logger.h"

class testDbUav : public CxxTest::TestSuite
{
	DbUavController* dbUavController;
	bool isConnected;

public:
	void setUp()
	{
		dbUavController = new DbUavController();

		DBConnectionStruct parameters;
		parameters.dbName = "uavtest";
		parameters.host = "127.0.0.1";
		parameters.port = 3306;
		parameters.login = "uavtest";
		parameters.password = "uavtest";

		isConnected = dbUavController->connectToDB(parameters);
	}

	void tearDown() {
		delete dbUavController;
	}

	void testConnection()
	{
		TS_ASSERT_EQUALS(true, isConnected);
	}

	void testAddUavType()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING UAV TYPE
		UavType uavType;
		uavType.name = "testUavType";

		int newUavTypeId = dbUavController->addUavType(uavType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newUavTypeId);
	}

	void testAddUavRole()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING UAV FRIEND TYPE
		UavRole uavRole;
		uavRole.name = "testFriend";
		uavRole.code = "testFriend";

		int newUavRoleId = dbUavController->addUavRole(uavRole);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newUavRoleId);

		// ADDING UAV ENEMY TYPE
		uavRole.name = "testEnemy";
		uavRole.code = "testEnemy";

		newUavRoleId = dbUavController->addUavRole(uavRole);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newUavRoleId);
	}

	void testAddUav()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// GETTING UAV ROLE
		UavRole uavRole;
		uavRole.code = "testEnemy";

		int uavRoleId = dbUavController->getUavRoleByCode(uavRole.code).id;
		TS_ASSERT_DIFFERS(INVALID_INDEX, uavRoleId);

		// GETTING UAV TYPE
		UavType uavType;
		uavType.name = "testUavType";

		int uavTypeId = dbUavController->getUavTypeByName(uavType.name);
		TS_ASSERT_DIFFERS(INVALID_INDEX, uavTypeId);

		// ADDING UAV
		Uav uav; // ;)
		uav.uavId = 8000;
		uav.ip = "10.11.12.134";
		uav.name = "TEST_UAV";
		uav.uavTypeId = uavTypeId;
		uav.roleId = uavRoleId;

		int newUavId = dbUavController->addUav(uav);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newUavId);
	}

	void testAddTargetType()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		TargetType targetType;
		targetType.name = "testTargetType";

		int newTargetTypeId = dbUavController->addTargetType(targetType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newTargetTypeId);
	}

	void testAddTarget()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		Target target;
		target.uavId = dbUavController->getUavByUavId(8000).id;
		target.ip = "127.1.1.1";
		target.port = 2000;
		target.type = dbUavController->getTargetTypeByName("testTargetType");

		int targetId = dbUavController->addTarget(target);
		TS_ASSERT_DIFFERS(INVALID_INDEX, targetId);
	}

	void testAddUavMission()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		UavMission mission;
		mission.uavId = dbUavController->getUavByUavId(8000).id;

		QList<Target> targets;
		bool isTargetsGetOk = dbUavController->getTargetsByType(
					dbUavController->getTargetTypeByName("testTargetType"), targets);
		TS_ASSERT_EQUALS(true, isTargetsGetOk);

		mission.targetId = targets.at(0).id;
		mission.regionCenterLat = 29.0f;
		mission.regionCenterLon = 59.0f;
		mission.regionCenterAltitude = 100.0f;
		mission.regionRadius = 1000.0f;
		mission.timeToTarget = QTime(2, 0);

		int missionId = dbUavController->addUavMission(mission);
		TS_ASSERT_DIFFERS(INVALID_INDEX, missionId);
	}


	void testAddDeviceType()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		DeviceType deviceType;
		deviceType.name = "testDeviceType";

		int deviceTypeId = dbUavController->addDeviceType(deviceType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, deviceTypeId);
	}

	void testAddDevice()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		Devices device;
		device.port = 1000;
		device.uavId = 8000;
		device.deviceId = 1;

		int deviceId = dbUavController->addDevice(device);
		TS_ASSERT_DIFFERS(INVALID_INDEX, deviceId);
	}

	void testAddStatusType()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		Status status;
		status.status = "testStatus";

		int statusId = dbUavController->addStatus(status);
		TS_ASSERT_DIFFERS(INVALID_INDEX, statusId);
	}

	void testAddUavInfo()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		QList<Devices> devices;
		bool isDevicesGetOk = dbUavController->getDevicesByType(dbUavController->getDeviceTypeByName("testDeviceType"), devices);
		TS_ASSERT_EQUALS(true, isDevicesGetOk);

		UavInfo uavInfo;
		uavInfo.uavId = 1; // FK
		uavInfo.device = devices.at(0).id; // FK
		uavInfo.lat = 1.0f;
		uavInfo.lon = 2.0f;
		uavInfo.alt = 300.0f;
		uavInfo.speed = 330.0f;
		uavInfo.yaw = 200.0f;
		uavInfo.restTime = QTime(1, 0);
		uavInfo.statusId = 1; // FK
		uavInfo.dateTime = QDateTime::currentDateTime();

		int newUavInfoId = dbUavController->addUavInfo(uavInfo);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newUavInfoId);
	}
};

#endif // TESTDB_H
