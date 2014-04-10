#ifndef DBBLAMANAGER_H
#define DBBLAMANAGER_H

#include <QObject>

#include "DbUavController.h"
#include "Interfaces/IDbUavManager.h"
#include "Interfaces/IUavDbChangedListener.h"
#include "BaseSubject.h"

class DbUavManager : public QObject, public IDbUavManager, public BaseSubject<IUavDbChangedListener>
{
	Q_OBJECT
private:
	IDbUavController* m_dbController;
	QMap<uint, Uav> m_knownUavsList;

public:
	explicit DbUavManager(QObject *parent = 0);

	void setDbController(IDbUavController*);

	int addUav(const Uav&);
	Uav getUavByUavId(const uint uavId);
	Uav getUav(const uint id);
	bool getUavsByRole(const QString& role, QList<Uav>& uavs);

	int addUavInfo(const UavInfo&);
	int getUavInfoByUavId(const uint uavId);

	int addDevice(const Devices&);
	bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords);

	int addUavMission(const UavMission&);
	bool getUavMissionsByUavId(const uint uavId, QList<UavMission>& missionsRecords);

	int addTarget(const Target&);
	bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords);
	bool getTargetsByUavId(const uint uavId, QList<Target>& targetsRecords);
	bool deleteTargetsByUavId(const uint uavId);

	int addTargetType(const TargetType&);
	int getTargetTypeByName(const QString&);

	int addUavType(const UavType&);
	int getUavTypeByName(const QString&);

	int addDeviceType(const DeviceType&);
	int getDeviceTypeByName(const QString&);

	int addStatus(const Status&);
	int getStatusByName(const QString&);

	int addUavRole(const UavRole&);
	UavRole getUavRole(const uint roleId);
	UavRole getUavRoleByName(const QString&);
	UavRole getUavRoleByCode(const QString&);
signals:

private slots:
	
};

#endif // DBBLAMANAGER_H
