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

public:
	void testConnection()
    {
		IDbControllerBase* dbUavController = new DbUavController(NULL);
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbUavController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		delete dbUavController;
    }

	void testAddUavType()
	{
		DbUavController* dbUavController = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbUavController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING UAV TYPE
		UavType uavType;
		uavType.name = "testUavType";

		int newUavTypeId = dbUavController->addUavType(uavType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newUavTypeId);

		delete dbUavController;
	}

	void testAddUavRole()
	{
		DbUavController* dbUavController = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbUavController->connectToDB(parameters);
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

		delete dbUavController;
	}

	void testAddUav()
	{
		DbUavController* dbUavController = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbUavController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// GETTING UAV TYPE
		UavRole uavRole;
		uavRole.code = "testEnemy";

		int uavRoleId = dbUavController->getUavRoleByCode(uavRole.code);
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

		delete dbUavController;
	}

	void testAddTargetType()
	{
		DbUavController* dbUavController = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbUavController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		TargetType targetType;
		targetType.name = "testTargetType";

		int newTargetTypeId = dbUavController->addTargetType(targetType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newTargetTypeId);

		delete dbUavController;
	}

	void testAddTarget()
	{
		DbUavController* db = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = db->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		Target target;
		target.ip = "127.1.1.1";
		target.port = 2000;
		target.type = db->getTargetTypeByName("testTargetType");

		int targetId = db->addTarget(target);
		TS_ASSERT_DIFFERS(INVALID_INDEX, targetId);

		delete db;
	}

	void testAddUavMission()
	{
		DbUavController* db = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = db->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		UavMission mission;
		mission.uavId = db->getUavByUavId(8000);

		QList<Target> targets;
		bool isTargetsGetOk = db->getTargetsByType(db->getTargetTypeByName("testTargetType"), targets);
		TS_ASSERT_EQUALS(true, isTargetsGetOk);

		mission.targetId = targets.at(0).id;
		mission.regionCenterLat = 29.0f;
		mission.regionCenterLon = 59.0f;
		mission.regionCenterAltitude = 100.0f;
		mission.regionRadius = 1000.0f;
		mission.timeToTarget = QTime(2, 0);

		int missionId = db->addUavMission(mission);
		TS_ASSERT_DIFFERS(INVALID_INDEX, missionId);

		delete db;
	}


	void testAddDeviceType()
	{
		DbUavController* dbUavController = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbUavController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		DeviceType deviceType;
		deviceType.name = "testDeviceType";

		int deviceTypeId = dbUavController->addDeviceType(deviceType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, deviceTypeId);

		delete dbUavController;
	}

	void testAddDevice()
	{
		DbUavController* dbUavController = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbUavController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		Devices device;
		device.port = 1000;
		device.uavId = 8000;
		device.deviceId = 1;

		int deviceId = dbUavController->addDevice(device);
		TS_ASSERT_DIFFERS(INVALID_INDEX, deviceId);

		delete dbUavController;
	}

	void testAddStatusType()
	{
		DbUavController* dbUavController = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbUavController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		Status status;
		status.status = "testStatus";

		int statusId = dbUavController->addStatus(status);
		TS_ASSERT_DIFFERS(INVALID_INDEX, statusId);

		delete dbUavController;
	}

	void testAddUavInfo()
	{
		DbUavController* db = new DbUavController();
		DBConnectionStruct parameters;

		parameters.dbName = "UAVTEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = db->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		QList<Devices> devices;
		bool isDevicesGetOk = db->getDevicesByType(db->getDeviceTypeByName("testDeviceType"), devices);
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

		int newUavInfoId = db->addUavInfo(uavInfo);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newUavInfoId);

		delete db;
	}
};

#endif // TESTDB_H
