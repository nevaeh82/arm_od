#include "DbUavManager.h"

DbUavManager::DbUavManager(int lifeTime, QObject *parent) :
	QObject(parent)
{
	m_dbController = NULL;

	setLifeTime( lifeTime );

	m_timeoutSignalMapper = new QSignalMapper(this);
	connect(m_timeoutSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(timeoutSlot(const QString &)));
}

DbUavManager::~DbUavManager()
{
}

void DbUavManager::setDbController(IDbUavController* dbController)
{
	m_dbController = dbController;
}

void DbUavManager::setLifeTime(int msecs)
{
	m_lifeTime = msecs < 1 ? MAX_LIFE_TIME : msecs;
}

int DbUavManager::addUav(const Uav &uav)
{
	return m_dbController->addUav(uav);
}

Uav DbUavManager::getUavByUavId(const uint uavId)
{
	return m_dbController->getUavByUavId(uavId);
}

Uav DbUavManager::getUav(const uint id)
{
	return m_dbController->getUav(id);
}

bool DbUavManager::getUavsByRole(const QString &role, QList<Uav> &uavs)
{
	return m_dbController->getUavsByRole(role, uavs);
}

int DbUavManager::addUavInfo(const UavInfo &uavInfo)
{
	QMutexLocker deleteLocker(&m_deleteMutex);

	//	  , _ ,
	//   ( o o )
	//  /'` ' `'\
	//  |'''''''|
	//  |\\'''//|
	//     """

	Uav uav = getUav(uavInfo.uavId);

	if (uav.id < 0) {
		return -1;
	}

	if (uav.name.isEmpty()) {
		uav.name = QString::number(uav.uavId);
	}

	QString uavRole = getUavRole(uav.roleId).code;
	QString key = QString::number(uav.uavId);

	if (!m_knownUavsList.contains(uav.uavId) && !m_lifeTimerMap.contains(key)) {

		m_knownUavsList.insert(uav.uavId, uav);

		QTimer* lifeTimer = new QTimer();
		connect(lifeTimer, SIGNAL(timeout()), m_timeoutSignalMapper, SLOT(map()));
		m_timeoutSignalMapper->setMapping(lifeTimer, key);
		m_lifeTimerMap.insert(key, lifeTimer);

		foreach (IUavDbChangedListener* receiver, m_receiversList){
			receiver->onUavAdded(uav, uavRole);
		}
	}

	//Need to update lifetimer
	QTimer* lifeTimer = m_lifeTimerMap.value(key, NULL);
	if (lifeTimer == NULL) {
		log_debug(QString("...Hmm... lifeTimer is NULL for %1...").arg(QString::number(uav.uavId)));
	} else {
		lifeTimer->start( m_lifeTime );
	}

	int newUavInfo = m_dbController->addUavInfo(uavInfo);
	UavInfo uavInfoForListeners = uavInfo;
	uavInfoForListeners.uavId = uav.uavId;

	if (newUavInfo != INVALID_INDEX){
		foreach (IUavDbChangedListener* receiver, m_receiversList){
			receiver->onUavInfoChanged(uavInfoForListeners, uavRole);
		}
	}

	return newUavInfo;
}

int DbUavManager::getUavInfoByUavId(const uint uavId)
{
	return m_dbController->getUavInfoByUavId(uavId);
}

int DbUavManager::addDevice(const Devices& device)
{
	return m_dbController->addDevice(device);
}

bool DbUavManager::getDevicesByType(const uint deviceTypeId, QList<Devices> &devicesRecords)
{
	return m_dbController->getDevicesByType(deviceTypeId, devicesRecords);
}

int DbUavManager::addUavMission(const UavMission &mission)
{
	return m_dbController->addUavMission(mission);
}

bool DbUavManager::getUavMissionsByUavId(const uint uavId, QList<UavMission> &missionsRecords)
{
	return m_dbController->getUavMissionsByUavId(uavId, missionsRecords);
}

int DbUavManager::addTarget(const Target& target)
{
	return m_dbController->addTarget(target);
}

bool DbUavManager::getTargetsByType(const uint targetTypeId, QList<Target> &targetsRecords)
{
	return m_dbController->getTargetsByType(targetTypeId, targetsRecords);
}

bool DbUavManager::getTargetsByUavId(const uint uavId, QList<Target> &targetsRecords)
{
	return m_dbController->getTargetsByUavId(uavId, targetsRecords);
}

bool DbUavManager::deleteTargetsByUavId(const uint uavId)
{
	return m_dbController->deleteTargetsByUavId(uavId);
}

int DbUavManager::addTargetType(const TargetType& targetType)
{
	return m_dbController->addTargetType(targetType);
}

int DbUavManager::getTargetTypeByName(const QString& name)
{
	return m_dbController->getTargetTypeByName(name);
}

int DbUavManager::addUavType(const UavType& uavType)
{
	return m_dbController->addUavType(uavType);
}

int DbUavManager::getUavTypeByName(const QString& name)
{
	return m_dbController->getUavTypeByName(name);
}

int DbUavManager::addDeviceType(const DeviceType& deviceType)
{
	return m_dbController->addDeviceType(deviceType);
}

int DbUavManager::getDeviceTypeByName(const QString& name)
{
	return m_dbController->getDeviceTypeByName(name);
}

int DbUavManager::addStatus(const Status& status)
{
	return m_dbController->addStatus(status);
}

int DbUavManager::getStatusByName(const QString& name)
{
	return m_dbController->getStatusByName(name);
}

int DbUavManager::addUavRole(const UavRole& role)
{
	return m_dbController->addUavRole(role);
}

UavRole DbUavManager::getUavRole(const uint roleId)
{
	return m_dbController->getUavRole(roleId);
}

UavRole DbUavManager::getUavRoleByName(const QString& name)
{
	return m_dbController->getUavRoleByName(name);
}

UavRole DbUavManager::getUavRoleByCode(const QString& code)
{
	return m_dbController->getUavRoleByCode(code);
}

void DbUavManager::deleteAllUav()
{
	QMutexLocker locker(&m_mutex);
	m_knownUavsList.clear();
}

void DbUavManager::onMethodCalled(const QString& method, const QVariant& argument)
{
	QByteArray data = argument.toByteArray();

	if (method == RPC_SLOT_SERVER_SEND_BLA_POINTS) {
		QDataStream inputDataStream(&data, QIODevice::ReadOnly);
		QVector<UAVPositionData> positionDataVector;
		inputDataStream >> positionDataVector;

		if (positionDataVector.size() < 1) {
			log_debug("Size uavpositiondata vector < 1");
			return;
		}

		/// Now we take first point, but we need to take more than 1 point
		UAVPositionData positionData = positionDataVector.at(0);
		addUavInfoToDb(positionData, OUR_UAV_ROLE, "UnknownUavType", "UnknownStatus", "UnknownDeviceType");
	} else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS) {
		sendEnemyUavPoints(data);
	} else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO) {
		sendEnemyUavPoints(data);
	}
}

void DbUavManager::addUavInfoToDb(const UAVPositionData& positionData, const QString& role, const QString& uavType, const QString& status, const QString& deviceType)
{
	QMutexLocker locker(&m_mutex);

	int uavId = getUavByUavId(positionData.boardID).id;
	if (uavId < 0){
		int uavUnknownTypeId = getUavTypeByName(uavType);

		if (uavUnknownTypeId < 0){
			UavType uavTypeStruct;
			uavTypeStruct.name = uavType;
			uavUnknownTypeId = addUavType(uavTypeStruct);
		}

		Uav uav;
		uav.uavId = positionData.boardID;
		uav.uavTypeId = uavUnknownTypeId;
		uav.ip = "127.0.0.1";

		UavRole uavRole = getUavRoleByCode(role);
		if (uavRole.id < 0){
			uavRole.code = role;
			uavRole.name = role;
			uavRole.id = addUavRole(uavRole);
		}
		uav.roleId = uavRole.id;

		uavId = addUav(uav);
	}

	int statusUnknownId = getStatusByName(status);
	if (statusUnknownId < 0){
		Status statusStruct;
		statusStruct.status = status;
		statusUnknownId = addStatus(statusStruct);
	}

	QList<Devices> devices;
	int unknownDeviceTypeId = getDeviceTypeByName(deviceType);

	if (unknownDeviceTypeId < 0){
		DeviceType deviceTypeStruct;
		deviceTypeStruct.name = deviceType;
		unknownDeviceTypeId = addDeviceType(deviceTypeStruct);
	}

	getDevicesByType(unknownDeviceTypeId, devices);

	int deviceUnknownId = -1;
	if (0 == devices.count()){
		Devices device;
		device.uavId = positionData.boardID;
		device.deviceId = unknownDeviceTypeId;
		device.port = 0;
		deviceUnknownId = addDevice(device);
	} else {
		deviceUnknownId = devices.at(0).id;
	}

	UavInfo uavInfo;
	uavInfo.uavId = uavId; // FK
	uavInfo.device = deviceUnknownId; // FK
	uavInfo.lat = positionData.latitude;
	uavInfo.lon = positionData.longitude;
	uavInfo.alt = positionData.altitude;
	uavInfo.speed = positionData.speed;
	uavInfo.yaw = positionData.course;
	uavInfo.restTime = QTime(1, 0);
	uavInfo.statusId = statusUnknownId; // FK
	uavInfo.dateTime = positionData.dateTime;

	addUavInfo(uavInfo);
}

void DbUavManager::sendEnemyUavPoints(const QByteArray& data)
{
	QByteArray inputData = data;
	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);
	UAVPositionDataEnemy uavEnemy;
	inputDataStream >> uavEnemy;

	addUavInfoToDb(uavEnemy, ENEMY_UAV_ROLE, "UnknownUavType", "UnknownStatus", "UnknownDeviceType");
}

void DbUavManager::addUavInfoToDb(const UAVPositionDataEnemy& positionDataEnemy, const QString &role, const QString &uavType, const QString &status, const QString &deviceType)
{
	UAVPositionData positionData;
	positionData.altitude = positionDataEnemy.altitude;
	positionData.course = positionDataEnemy.course;
	positionData.speed = positionDataEnemy.speed;
	positionData.state = positionDataEnemy.state;

	positionData.latitude=positionDataEnemy.track.at(0).x();
	positionData.longitude=positionDataEnemy.track.at(0).y();

	positionData.boardID = ENEMY_UAV_ID_OFFSET;

	addUavInfoToDb(positionData, role, uavType, status, deviceType);
}

void DbUavManager::timeoutSlot(const QString& key)
{
	QMutexLocker deleteLocker(&m_deleteMutex);

	Uav uav = m_knownUavsList.take(key.toUInt());
	QTimer* lifeTimer = m_lifeTimerMap.take(key);

	log_debug(QString("DELETING UAV BY ID = %1...").arg(QString::number(uav.uavId)));

	QString uavRole = getUavRole(uav.roleId).code;
	foreach (IUavDbChangedListener* receiver, m_receiversList){
		receiver->onUavRemoved(uav, uavRole);
	}

	if (lifeTimer != NULL){
		lifeTimer->stop();
		lifeTimer->deleteLater();
	}
}
