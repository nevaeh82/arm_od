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

		TS_ASSERT_DIFFERS(INVALID_INDEX, target.uavId);
		TS_ASSERT_DIFFERS(INVALID_INDEX, target.type);

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
		TS_ASSERT_DIFFERS(0, targets.length());

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

	void testAddSourceType()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		SourceType sourceType;
		sourceType.name = "testSourceType";

		int sourceTypeId = dbUavController->addSourceType(sourceType);
		TS_ASSERT_DIFFERS(INVALID_INDEX, sourceTypeId);
	}

	void testAddSource()
	{
		TS_ASSERT_EQUALS(true, isConnected);

		// ADDING TARGET TYPE
		Sources source;
		source.sourceId = 555;
		source.sourceTypeId = 1;

		int sourceId = dbUavController->addSource(source);
		TS_ASSERT_DIFFERS(INVALID_INDEX, sourceId);
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
		TS_ASSERT_DIFFERS(true, devices.isEmpty());

		QList<Sources> sources;
		bool isSourcesGetOk = dbUavController->getSourceByType(dbUavController->getSourceTypeByName("testSourceType"), sources);
		TS_ASSERT_EQUALS(true, isSourcesGetOk);
		TS_ASSERT_DIFFERS(true, sources.isEmpty());

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
		uavInfo.source = sources.at(0).id; // FK
		uavInfo.dateTime = QDateTime::currentDateTime();

		int newUavInfoId = dbUavController->addUavInfo(uavInfo);
		TS_ASSERT_DIFFERS(INVALID_INDEX, newUavInfoId);
	}

	void testGetTargetsByMission()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		QList<Target> targetList;
		//append Target for test
		Target target;
		target.ip = "127.1.1.1";
		target.port = 1000;
		target.type = 1;
		target.uavId = 1;
		int targetId = dbUavController->addTarget(target);
		TS_ASSERT_DIFFERS( INVALID_INDEX, targetId );

		//append Mission for test
		UavMission mission;
		mission.regionCenterAltitude = 0.0;
		mission.regionCenterLat = 0.0;
		mission.regionCenterLon = 0.0;
		mission.regionRadius = 0.0;
		mission.targetId = targetId;
		mission.timeToTarget = QTime(0,0,0);
		mission.uavId = 1;
		int missionId = dbUavController->addUavMission(mission);
		TS_ASSERT_DIFFERS( INVALID_INDEX, missionId );

		bool getOk = dbUavController->getTargetsByMission(missionId, targetList);
		TS_ASSERT_EQUALS( true, getOk );
		TS_ASSERT_DIFFERS( 0, targetList.size() );

		//remove test objects
		dbUavController->deleteUavMissionsByUavId(1);
		dbUavController->deleteTargetsById(targetId);
	}

	void testDeleteUavMissionByUavId()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		//append Mission and Target for test
		const uint uavId = 1;

		//append Target
		Target target;
		target.ip = "127.1.1.1";
		target.port = 1000;
		target.type = 1;
		target.uavId = 1;
		int targetId = dbUavController->addTarget(target);

		UavMission mission;
		mission.regionCenterAltitude = 0.0;
		mission.regionCenterLat = 0.0;
		mission.regionCenterLon = 0.0;
		mission.regionRadius = 0.0;
		mission.targetId = targetId;
		mission.timeToTarget = QTime(0,0,0);
		mission.uavId = uavId;
		int missionId = dbUavController->addUavMission(mission);
		TS_ASSERT_DIFFERS( INVALID_INDEX, missionId );

		bool result = dbUavController->deleteUavMissionsByUavId(uavId);
		TS_ASSERT_EQUALS( true, result );

		//removing extra target
		dbUavController->deleteTargetsById(targetId);
	}

	void testDeleteTargetsById()
	{
		TS_ASSERT_EQUALS( true, isConnected );

		//append Target for test
		Target target;
		target.ip = "127.1.1.1";
		target.port = 1000;
		target.type = 1;
		target.uavId = 1;
		int targetId = dbUavController->addTarget(target);
		TS_ASSERT_DIFFERS( INVALID_INDEX, targetId );

		bool result = dbUavController->deleteTargetsById(targetId);
		TS_ASSERT_EQUALS( true, result );
	}
};

#endif // TESTDB_H
