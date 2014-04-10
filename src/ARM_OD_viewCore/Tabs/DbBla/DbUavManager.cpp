#include "DbUavManager.h"

DbUavManager::DbUavManager(QObject *parent) :
	QObject(parent)
{
	m_dbController = NULL;
}

void DbUavManager::setDbController(IDbUavController* dbController)
{
	m_dbController = dbController;
}

int DbUavManager::addUav(const Uav &uav)
{
	int newUavId = m_dbController->addUav(uav);

	return newUavId;
}

Uav DbUavManager::getUavByUavId(const uint uavId)
{
	return m_dbController->getUavByUavId(uavId);
}

Uav DbUavManager::getUav(const uint id)
{
	return m_dbController->getUav(id);
}

int DbUavManager::addUavInfo(const UavInfo &uavInfo)
{
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

	/// TODO: fix it
	if (ENEMY_UAV_ROLE == uavRole) {
		uav.uavId = 50;
		uav.name = QString::number(uav.uavId);
	}

	if (!m_knownUavsList.contains(uav.uavId)) {
		m_knownUavsList.insert(uav.uavId, uav);

		foreach (IUavDbChangedListener* receiver, m_receiversList){
			receiver->onUavAdded(uav, uavRole);
		}
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
