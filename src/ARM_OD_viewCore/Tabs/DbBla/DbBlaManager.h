#ifndef DBBLAMANAGER_H
#define DBBLAMANAGER_H

#include <QObject>
#include <QMap>

#include "DbBlaController.h"
#include "Interfaces/IDbBlaManager.h"
#include "Interfaces/IBlaDbChangedListener.h"
#include "BaseSubject.h"

class DbBlaManager : public QObject, public IDbBlaManager, public BaseSubject<IBlaDbChangedListener>
{
	Q_OBJECT
private:
	IDbBlaController* m_dbController;
	QMap<uint, Bla> m_knownUavsList;

public:
	explicit DbBlaManager(QObject *parent = 0);

	void setDbController(IDbBlaController*);

	int addBla(const Bla&);
	Bla getBlaByBlaId(const uint blaId);
	Bla getBla(const uint id);

	int addBlaInfo(const BlaInfo&);
	int getBlaInfoByBlaId(const uint blaId);

	int addDevice(const Devices&);
	bool getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords);

	int addBlaMission(const BlaMission&);
	bool getBlaMissionsByBlaId(const uint blaId, QList<BlaMission>& missionsRecords);

	int addTarget(const Target&);
	bool getTargetsByType(const uint targetTypeId, QList<Target>& targetsRecords);

	int addTargetType(const TargetType&);
	int getTargetTypeByName(const QString&);

	int addBlaType(const BlaType&);
	int getBlaTypeByName(const QString&);

	int addDeviceType(const DeviceType&);
	int getDeviceTypeByName(const QString&);

	int addStatus(const Status&);
	int getStatusByName(const QString&);

signals:

private slots:
	
};

#endif // DBBLAMANAGER_H
