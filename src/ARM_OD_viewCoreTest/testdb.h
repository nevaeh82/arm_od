#ifndef TESTDB_H
#define TESTDB_H

#include <cxxtest/TestSuite.h>

#include <QTime>

#include "Tabs/DbBla/DbBlaController.h"
#include "Tabs/DbBla/Defines.h"
#include "Db/Defines.h"
#include "Interfaces/IDbControllerBase.h"

#include "Logger.h"

class testDbBla : public CxxTest::TestSuite
{

public:
	void testConnection()
    {
		IDbControllerBase* dbBlaController = new DbBlaController(NULL);
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbBlaController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		delete dbBlaController;
    }

	void testAddBlaType()
	{
		DbBlaController* dbBlaController = new DbBlaController();
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbBlaController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING BLA TYPE
		BlaType blaType;
		blaType.name = "testBlaType";

		int newBlaTypeId = dbBlaController->addBlaType(blaType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newBlaTypeId);

		delete dbBlaController;
	}

	void testAddBla()
	{
		DbBlaController* dbBlaController = new DbBlaController();
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbBlaController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// GETTING BLA TYPE
		BlaType blaType;
		blaType.name = "testBlaType";

		int blaTypeId = dbBlaController->getBlaTypeByName(blaType.name);
		TS_ASSERT_DIFFERS(INVALID_INDEX, blaTypeId);


		// ADDING BLA
		Bla bla; // ;)
		bla.blaId = 8000;
		bla.ip = "10.11.12.134";
		bla.name = "TEST_BLA";
		bla.type = blaTypeId;

		int newBlaId = dbBlaController->addBla(bla);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newBlaId);

		delete dbBlaController;
	}

	void testAddTargetType()
	{
		DbBlaController* dbBlaController = new DbBlaController();
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbBlaController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		TargetType targetType;
		targetType.name = "testTargetType";

		int newTargetTypeId = dbBlaController->addTargetType(targetType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newTargetTypeId);

		delete dbBlaController;
	}

	void testAddTarget()
	{
		DbBlaController* db = new DbBlaController();
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
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
		target.targetId = 8001;
		target.type = db->getTargetTypeByName("testTargetType");

		int statusId = db->addTarget(target);
		TS_ASSERT_DIFFERS(INVALID_INDEX, statusId);

		delete db;
	}

	void testAddBlaMission()
	{
		DbBlaController* db = new DbBlaController();
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = db->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		BlaMission mission;
		mission.blaId = db->getBlaByBlaId(8000);

		QList<Target> targets;
		bool isTargetsGetOk = db->getTargetsByType(db->getTargetTypeByName("testTargetType"), targets);
		TS_ASSERT_EQUALS(true, isTargetsGetOk);

		mission.targetId = targets.at(0).id;
		mission.regionCenterLat = 29.0f;
		mission.regionCenterLon = 59.0f;
		mission.regionCenterAtitude = 100.0f;
		mission.regionRadius = 1000.0f;
		mission.timeToTarget = QTime(2, 0);

		int missionId = db->addBlaMission(mission);
		TS_ASSERT_DIFFERS(INVALID_INDEX, missionId);

		delete db;
	}


	void testAddDeviceType()
	{
		DbBlaController* dbBlaController = new DbBlaController();
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbBlaController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		DeviceType deviceType;
		deviceType.name = "testDeviceType";

		int deviceTypeId = dbBlaController->addDeviceType(deviceType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, deviceTypeId);

		delete dbBlaController;
	}

	void testAddDevice()
	{
		DbBlaController* dbBlaController = new DbBlaController();
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbBlaController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		Devices device;
		device.port = 1000;
		device.blaId = 8000;
		device.deviceId = 1;

		int deviceId = dbBlaController->addDevice(device);
		TS_ASSERT_DIFFERS(INVALID_INDEX, deviceId);

		delete dbBlaController;
	}

	void testAddStatusType()
	{
		DbBlaController* dbBlaController = new DbBlaController();
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = dbBlaController->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		Status status;
		status.status = "testStatus";

		int statusId = dbBlaController->addStatus(status);
		TS_ASSERT_DIFFERS(INVALID_INDEX, statusId);

		delete dbBlaController;
	}

	void testAddBlaInfo()
	{
		DbBlaController* db = new DbBlaController();
		DBConnectionStruct parameters;

		parameters.dbName = "BLATEST";
		parameters.host = "127.0.0.1";
		parameters.login = "root";
		parameters.port = 3306;
		parameters.password = "qwerty12345";
		bool isConnected = db->connectToDB(parameters);
		TS_ASSERT_EQUALS(true, isConnected);

		QList<Devices> devices;
		bool isDevicesGetOk = db->getDevicesByType(db->getDeviceTypeByName("testDeviceType"), devices);
		TS_ASSERT_EQUALS(true, isDevicesGetOk);

		BlaInfo blaInfo;
		blaInfo.blaId = 1; // FK
		blaInfo.device = devices.at(0).id; // FK
		blaInfo.lat = 1.0f;
		blaInfo.lon = 2.0f;
		blaInfo.alt = 300.0f;
		blaInfo.speed = 330.0f;
		blaInfo.yaw = 200.0f;
		blaInfo.restTime = QTime(1, 0);
		blaInfo.statusId = 1; // FK
		blaInfo.dateTime = QDateTime::currentDateTime();

		int newBlaInfoId = db->addBlaInfo(blaInfo);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newBlaInfoId);

		delete db;
	}
};

#endif // TESTDB_H
