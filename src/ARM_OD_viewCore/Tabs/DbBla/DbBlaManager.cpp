#include "DbBlaManager.h"

DbBlaManager::DbBlaManager(QObject *parent) :
	QObject(parent)
{
	m_dbController = NULL;
}

void DbBlaManager::setDbController(IDbBlaController* dbController)
{
	m_dbController = dbController;
}

int DbBlaManager::addBla(const Bla& bla)
{
	int newBlaId = m_dbController->addBla(bla);

	/// NOW: sent after receiving first UAV info

//	if (newBlaId != INVALID_INDEX){
//		foreach (IBlaDbChangedListener* receiver, m_receiversList){
//			receiver->onBlaAdded(bla);
//		}
//	}

	return newBlaId;
}

Bla DbBlaManager::getBlaByBlaId(const uint blaId)
{
	return m_dbController->getBlaByBlaId(blaId);
}

Bla DbBlaManager::getBla(const uint id)
{
	return m_dbController->getBla(id);
}

int DbBlaManager::addBlaInfo(const BlaInfo& blaInfo)
{
	Bla bla = getBla(blaInfo.blaId);

	if (bla.id < 0) {
		return -1;
	}

	if (bla.name.isEmpty()) {
		bla.name = QString::number(bla.blaId);
	}

	if (!m_knownUavsList.contains(bla.blaId)) {
		m_knownUavsList.insert(bla.blaId, bla);

		foreach (IBlaDbChangedListener* receiver, m_receiversList){
			receiver->onBlaAdded(bla);
		}
	}

	int newBlaInfo = m_dbController->addBlaInfo(blaInfo);

	//o0

//	  , _ ,
//   ( o o )
//  /'` ' `'\
//  |'''''''|
//  |\\'''//|
//     """
	BlaInfo blaInfoForListeners = blaInfo;
	blaInfoForListeners.blaId = bla.blaId;

	if (newBlaInfo != INVALID_INDEX){
		foreach (IBlaDbChangedListener* receiver, m_receiversList){
			receiver->onBlaInfoChanged(blaInfoForListeners);
		}
	}

	return newBlaInfo;
}

int DbBlaManager::getBlaInfoByBlaId(const uint blaId)
{
	return m_dbController->getBlaInfoByBlaId(blaId);
}

int DbBlaManager::addDevice(const Devices& device)
{
	return m_dbController->addDevice(device);
}

bool DbBlaManager::getDevicesByType(const uint deviceTypeId, QList<Devices> &devicesRecords)
{
	return m_dbController->getDevicesByType(deviceTypeId, devicesRecords);
}

int DbBlaManager::addBlaMission(const BlaMission& mission)
{
	return m_dbController->addBlaMission(mission);
}

bool DbBlaManager::getBlaMissionsByBlaId(const uint blaId, QList<BlaMission> &missionsRecords)
{
	return m_dbController->getBlaMissionsByBlaId(blaId, missionsRecords);
}

int DbBlaManager::addTarget(const Target& target)
{
	return m_dbController->addTarget(target);
}

bool DbBlaManager::getTargetsByType(const uint targetTypeId, QList<Target> &targetsRecords)
{
	return m_dbController->getTargetsByType(targetTypeId, targetsRecords);
}

int DbBlaManager::addTargetType(const TargetType& targetType)
{
	return m_dbController->addTargetType(targetType);
}

int DbBlaManager::getTargetTypeByName(const QString& name)
{
	return m_dbController->getTargetTypeByName(name);
}

int DbBlaManager::addBlaType(const BlaType& blaType)
{
	return m_dbController->addBlaType(blaType);
}

int DbBlaManager::getBlaTypeByName(const QString& name)
{
	return m_dbController->getBlaTypeByName(name);
}

int DbBlaManager::addDeviceType(const DeviceType& deviceType)
{
	return m_dbController->addDeviceType(deviceType);
}

int DbBlaManager::getDeviceTypeByName(const QString& name)
{
	return m_dbController->getDeviceTypeByName(name);
}

int DbBlaManager::addStatus(const Status& status)
{
	return m_dbController->addStatus(status);
}

int DbBlaManager::getStatusByName(const QString& name)
{
	return m_dbController->getStatusByName(name);
}
