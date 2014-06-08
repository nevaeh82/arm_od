#ifndef DBBLAMANAGER_H
#define DBBLAMANAGER_H

#include <QObject>
#include <QSignalMapper>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include <Interfaces/IRpcListener.h>
#include <QFile>
#include <QDir>

#include <logManager/LogManager.h>

#include "Tabs/DbBla/DbUavController.h"

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

	int m_lifeTime;

	IUavHistory* m_uavHistory;

	QFile* fi;
	LogManager* m_logManager;

public:
	explicit DbUavManager(int lifeTime = MAX_LIFE_TIME, QObject *parent = 0);
	virtual ~DbUavManager();

	QMap<uint, Uav> getKnownUavList();

	void setDbController(IDbUavController*);
	void setLifeTime(int msecs);

	int addUav(const Uav&);
	Uav getUavByUavId(const uint uavId);
	Uav getUav(const uint id);
	bool getUavsByRole(const QString& role, QList<Uav>& uavs);

	int addUavInfo(const UavInfo&);
	bool getUavInfoByUavId(const uint uavId, QList<UavInfo>& uavInfoList);

	IUavHistory* getUavHistory();

	int addDevice(const Devices&);
	bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords);

	int addSource(const Source&);
	int getSourceId(const uint sourceId, const uint sourceTypeId);
	bool getSourcesByType(const uint sourceId, QList<Source>& sourcesRecords);
	Source getSource(const uint sourceId);

	int addUavMission(const UavMission&);
	bool getUavMissionsByUavId(const uint uavId, QList<UavMission>& missionsRecords);
	bool deleteUavMissionsByUavId(const uint uavId);
	bool deleteUavMissionsByTargetId(const uint targetId);

	int addTarget(const Target&);
	bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords);
	bool getTargetsByUavId(const uint uavId, QList<Target>& targetsRecords);
	bool getTargetsByUavIdAndTargetType(const uint uavId, const QString& targetType, QList<Target>& targetsRecords);
	bool deleteTargetsByUavId(const uint uavId);
	bool deleteTargetsById(const uint id);

	int addTargetType(const TargetType&);
	int getTargetTypeByName(const QString&);

	int addUavType(const UavType&);
	int getUavTypeByName(const QString&);

	int addDeviceType(const DeviceType&);
	int getDeviceTypeByName(const QString&);

	int addSourceType(const SourceType&);
	int getSourceTypeByName(const QString&);

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
	void addUavInfoToDb(const UAVPositionData& positionData, const QString& role,
						const QString& uavType, const QString& status, const QString& deviceType, const QString &sourceType);
	void sendEnemyUavPoints(const QByteArray& data, uint sourceType);
	void addUavInfoToDb(const UAVPositionDataEnemy& positionDataEnemy, const QString &role,
						const QString &uavType, const QString &status, const QString &deviceType, const QString &sourceType);

	QString getEnemySourceTypeName(uint sourceType);

private slots:
	void timeoutSlot(const QString& key);
};

#endif // DBBLAMANAGER_H
