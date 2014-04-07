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

public:
	explicit DbUavManager(QObject *parent = 0);

	void setDbController(IDbUavController*);

	int addUav(const Uav&);
	int getUavByUavId(const uint uavId);

	int addUavInfo(const UavInfo&);
	int getUavInfoByUavId(const uint uavId);

	int addDevice(const Devices&);
	bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords);

	int addUavMission(const UavMission&);
	bool getUavMissionsByUavId(const uint uavId, QList<UavMission>& missionsRecords);

	int addTarget(const Target&);
	bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords);

	int addTargetType(const TargetType&);
	int getTargetTypeByName(const QString&);

	int addUavType(const UavType&);
	int getUavTypeByName(const QString&);

	int addDeviceType(const DeviceType&);
	int getDeviceTypeByName(const QString&);

	int addStatus(const Status&);
	int getStatusByName(const QString&);

	int addUavRole(const UavRole&);
	UavRole getUavRoleByName(const QString&);
	UavRole getUavRoleByCode(const QString&);
signals:

private slots:
	
};

#endif // DBBLAMANAGER_H
