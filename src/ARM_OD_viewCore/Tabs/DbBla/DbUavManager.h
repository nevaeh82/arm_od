#ifndef DBBLAMANAGER_H
#define DBBLAMANAGER_H

#include <QObject>
#include <QSignalMapper>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include <Interfaces/IRpcListener.h>

#include "DbUavController.h"
#include "Interfaces/IDbUavManager.h"
#include "Interfaces/IUavDbChangedListener.h"
#include "BaseSubject.h"

#include "RPC/RpcDefines.h"
#include "UAVDefines.h"

/// 5 minutes	= 300000 msec
/// 10 minutes	= 600000 msec
#define MAX_LIFE_TIME	600000

class DbUavManager : public QObject, public IDbUavManager, public BaseSubject<IUavDbChangedListener>, public IRpcListener
{
	Q_OBJECT
private:
	IDbUavController* m_dbController;

	QMap<uint, Uav> m_knownUavsList;
	QMap<QString, QTimer*> m_lifeTimerMap;
	QSignalMapper* m_timeoutSignalMapper;
	QMutex m_deleteMutex;

	QMutex m_mutex;

public:
	explicit DbUavManager(QObject *parent = 0);
	virtual ~DbUavManager();

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

	void deleteAllUav();

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

private:
	void addUavInfoToDb(const UAVPositionData& positionData, const QString& role, const QString& uavType, const QString& status, const QString& deviceType);
	void sendEnemyUavPoints(const QByteArray& data);
	void addUavInfoToDb(const UAVPositionDataEnemy& positionDataEnemy, const QString &role, const QString &uavType, const QString &status, const QString &deviceType);

private slots:
	void timeoutSlot(const QString& key);
};

#endif // DBBLAMANAGER_H
