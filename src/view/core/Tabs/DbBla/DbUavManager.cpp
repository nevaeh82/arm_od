#include <QDebug>

#include "DbUavManager.h"
#include "SolverExchange.h"
//#include "SolverPacket1.pb.h"

#define LIFETIME_1 20000
#define LIFETIME_2 3000

DbUavManager::DbUavManager(int lifeTime, QObject *parent) :
	QObject(parent)
{
	QDir dir;
	dir.mkdir("./logs/SpecialLogs");
	m_logManager = new LogManager("./logs/SpecialLogs/logUAVs.log");

	m_inRun = false;

	if(!m_logManager)
	{
		log_debug("error");
	}

	m_dbController = NULL;
	m_uavHistory = NULL;

	setLifeTime( lifeTime );

	m_timeoutSignalMapper = new QSignalMapper(this);
	connect(m_timeoutSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(timeoutSlot(const QString &)));

	connect(this, SIGNAL(signalMethodCalled(QString,QVariant)), this, SLOT(onMethodCalledInternal(QString,QVariant)), Qt::QueuedConnection);
}

DbUavManager::~DbUavManager()
{
	delete m_logManager;
}

QMap<QString, Uav> DbUavManager::getKnownUavList()
{
	return m_knownUavsList;
}

void DbUavManager::setDbController(IDbUavController* dbController)
{
	m_dbController = dbController;
}

void DbUavManager::setLifeTime(int msecs)
{
	m_lifeTime = msecs < 1 ? MAX_LIFE_TIME : msecs;

	if( m_uavHistory != NULL ) {
		m_uavHistory->setLifeTime( m_lifeTime );
}
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

int DbUavManager::addUavInfo(const UavInfo &uavInfo, bool actual,
							 const QVector<QPointF>& tail,
							 const QVector<QPointF>& tailStdDev,
							 const QVector<QDateTime> &tailTime)
{
	QMutexLocker deleteLocker(&m_deleteMutex);

	//	  , _ ,
	//   ( o o )
	//  /'` ' `'\
	//  |'''''''|\
	//  |\\'''//|\
	//     """\

	Uav uav = getUav(uavInfo.uavId);

//return if db fail
//	if (uav.id < 0) {
//		return -1;
//	}


	if(uav.id < 0) {
		uav.id = uavInfo.id;
		uav.uavId = uavInfo.uavId;
		uav.ip = "";
		uav.uavTypeId = 0;
		uav.roleId = 0;
		uav.name = uavInfo.name;
		uav.freqId = 0;
		uav.historical = false;

	}

	if (uav.name.isEmpty()) {
		uav.name = uavInfo.name;
	}

	// If current record is not actual information about BPLA
	// we try just to insert/update uav info record
	/// todo: commented because of perfomance issue
    int recordIndex = 0;// m_dbController->addUavInfo(uavInfo);
//
    if (!actual) {
        return recordIndex;
    }

	//m_dbController->addUavInfo(uavInfo);

	if(tail.size() <= 0) {
		m_dbController->addUavInfo(uavInfo);
	} else {

		UavInfo uavInfoTmp = uavInfo;
		for(int i = 0; i < tail.size(); i++) {

			uavInfoTmp.lat = tail.at(i).x();
			uavInfoTmp.lon = tail.at(i).y();

			uavInfoTmp.latStddev = tailStdDev.at(i).x();
			uavInfoTmp.lonStddev = tailStdDev.at(i).y();

			uavInfoTmp.dateTime = tailTime.at(i);

			m_dbController->addUavInfo(uavInfoTmp);
		}

	}

	/// todo: uncomment upper code and delete this after perfomance issue will be solved
	if (!actual) return -1;

	QString uavRole = getUavRole(uav.roleId).code;
    QString key = uav.name;

	if(uavRole.isEmpty()) {
		uavRole = uavInfo.role;
	}

	if (!m_knownUavsList.contains(key) && !m_lifeTimerMap.contains(key)) {
		m_knownUavsList.insert(key, uav);

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
		lifeTimer->start( LIFETIME_1 );
	}

	UavInfo uavInfoForListeners = uavInfo;
	uavInfoForListeners.uavId = uav.uavId;

	uavInfoForListeners.source = getSource(uavInfo.source).sourceId;

	if(uavInfoForListeners.source == 0) {
		uavInfoForListeners.source = uavInfo.source;
	}

	//if (recordIndex != INVALID_INDEX){
		foreach (IUavDbChangedListener* receiver, m_receiversList) {
			receiver->onUavInfoChanged(uavInfoForListeners, uavRole, tail, tailStdDev);
		}
	//}

	return recordIndex;
}

bool DbUavManager::getUavInfoByUavId(const uint uavId, QList<UavInfo>& uavInfoList)
{
	return m_dbController->getUavInfoByUavId(uavId, uavInfoList);
}

IUavHistory* DbUavManager::getUavHistory()
{
	if( m_uavHistory == NULL ) {
		m_uavHistory = m_dbController->getUavHistory();
		m_uavHistory->setLifeTime( m_lifeTime );
	}

	return m_uavHistory;
}

int DbUavManager::addDevice(const Devices& device)
{
	return m_dbController->addDevice(device);
}

bool DbUavManager::getDevicesByType(const uint deviceTypeId, QList<Devices> &devicesRecords)
{
	return m_dbController->getDevicesByType(deviceTypeId, devicesRecords);
}

int DbUavManager::addSource(const Source & source)
{
	return m_dbController->addSource(source);
}

int DbUavManager::getSourceId(const uint sourceId, const uint sourceTypeId)
{
	return m_dbController->getSourceId(sourceId, sourceTypeId);
}

bool DbUavManager::getSourcesByType(const uint sourceId, QList<Source> &sourcesRecords)
{
	return m_dbController->getSourceByType(sourceId, sourcesRecords);
}

Source DbUavManager::getSource(const uint sourceId)
{
	return m_dbController->getSource(sourceId);
}

int DbUavManager::addUavMission(const UavMission &mission)
{
	return m_dbController->addUavMission(mission);
}

bool DbUavManager::getUavMissionsByUavId(const uint uavId, QList<UavMission> &missionsRecords)
{
	return m_dbController->getUavMissionsByUavId(uavId, missionsRecords);
}

bool DbUavManager::deleteUavMissionsByUavId(const uint uavId)
{
	return m_dbController->deleteUavMissionsByUavId(uavId);
}

bool DbUavManager::deleteUavMissionsByTargetId(const uint targetId)
{
	return m_dbController->deleteUavMissionsByTargetId(targetId);
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

bool DbUavManager::getTargetsByUavIdAndTargetType(const uint uavId, const QString& targetType, QList<Target>& targetsRecords)
{
	QList<UavMission> uavMissions;
	m_dbController->getUavMissionsByUavId(uavId, uavMissions);

	foreach(UavMission mission, uavMissions){

		QList<Target> targets;
		m_dbController->getTargetsByMission(mission.id, targets);

		foreach(Target target, targets){
			if (target.type == m_dbController->getTargetTypeByName(targetType)){
				targetsRecords.append(target);
			}
		}
	}

	return true;
}

bool DbUavManager::deleteTargetsByUavId(const uint uavId)
{
	return m_dbController->deleteTargetsByUavId(uavId);
}

bool DbUavManager::deleteTargetsById(const uint id)
{
	return m_dbController->deleteTargetsById(id);
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

int DbUavManager::addSourceType(const SourceType & sourceType)
{
	return m_dbController->addSourceType(sourceType);
}

int DbUavManager::getSourceTypeByName(const QString & name)
{
	return m_dbController->getSourceTypeByName(name);
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
	foreach (QString key, m_lifeTimerMap.keys()) {
		timeoutSlot(key);
	}
	m_knownUavsList.clear();
	m_lifeTimerMap.clear();
}

void DbUavManager::onMethodCalledInternal(QString method, QVariant argument)
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

		QString dataToFile = QTime::currentTime().toString("hh:mm:ss:zzz") + " " + QString::number(positionDataVector.at(0).latitude) + " " + QString::number(positionDataVector.at(0).longitude) + " " + QString::number(positionDataVector.at(0).altitude) + "\n";
		m_logManager->writeToFile(dataToFile);

		/// Now we take first point, but we need to take more than 1 point
		UAVPositionData positionData = positionDataVector.at(0);
		positionData.name = QString::number(positionData.boardID);
		positionData.state = 1;
		addUavInfoToDb(positionData, OUR_UAV_ROLE, "UnknownUavType", "1", "UnknownDeviceType", "UnknownSourceType");
	} /*else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS) {
		sendEnemyUavPoints(data, UAV_SOLVER_MANUAL_SOURCE);
	}*/ else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS_1) {
		//debug
		int i = 0;

		m_inRun = true;

		SolverProtocol::Packet pkt;
		pkt.ParseFromArray( data.data(), data.size() );
		{
			QList<UAVPositionDataEnemy> uavList = getUavListSingleFromProto(pkt);

			if(!uavList.isEmpty()) {
				sendEnemyUavPoints(uavList, UAV_SOLVER_SINGLE_1_SOURCE);
			}

			QList<UAVPositionDataEnemy> uavList2 = getUavListSingleFromProto(pkt, true);

			if(!uavList2.isEmpty()) {
				sendEnemyUavPoints(uavList2, UAV_SOLVER_SINGLE_2_SOURCE);
			}

		}

		{
			pkt.has_datafromsolver();
			if(isSolverMessageHasTrajectoryManual(pkt)) {

				emit protoToNiipp(data);

				for(int i = 0; i < pkt.datafromsolver().solution_manual_altitude().trajectory_size(); i++) {
					QList<UAVPositionDataEnemy> uavList = getUavListTrajectoryFromProto(
								pkt.datafromsolver().solution_manual_altitude().trajectory(i));

					if(uavList.isEmpty()) {
						continue;
					}

					sendEnemyUavPoints(uavList, UAV_SOLVER_MANUAL_SOURCE);
				}

			}

			if(isSolverMessageHasTrajectoryAuto(pkt)) {

				emit protoToNiipp(data);

				for(int i = 0; i < pkt.datafromsolver().solution_automatic_altitude().trajectory_size(); i++) {
					QList<UAVPositionDataEnemy> uavList = getUavListTrajectoryFromProto(
								pkt.datafromsolver().solution_automatic_altitude().trajectory(i));

					if(uavList.isEmpty()) {
						continue;
					}

					sendEnemyUavPoints(uavList, UAV_SOLVER_AUTO_SOURCE);
				}
			}
		}


		m_inRun = false;
//		sendEnemyUavPoints(data, UAV_SOLVER_SOURCE_1);
		// Read Protobuf, revreate data from new protobuf to
		// OD uav struct.
		//Write to database, and do not send on map
	}
	else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO) {
		sendEnemyUavPoints(data, UAV_SOLVER_AUTO_SOURCE);
	} else if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS_SINGLE) {
		sendEnemyUavPoints(data, UAV_SOLVER_SINGLE_1_SOURCE);
	}
}

void DbUavManager::onMethodCalled(const QString& method, const QVariant& argument)
{
	//log_debug(QString("OK::::: IN RUN %1  >>>>>>>>>>>>").arg(m_inRun));

	if(m_inRun) {
		return;  //Drop if db is writing in
	}

	emit signalMethodCalled(method, argument);
}

QList<UAVPositionDataEnemy> DbUavManager::getUavListSingleFromProto(SolverProtocol::Packet pkt, bool autoAlt)
{
    QList<UAVPositionDataEnemy> retList = QList<UAVPositionDataEnemy>();

    pkt.has_datafromsolver();

	if(!autoAlt && !isSolverMessageHasSingleMarksManual(pkt)) {
        return retList;
    }

	if(autoAlt && !isSolverMessageHasSingleMarksAuto(pkt)) {
		return retList;
	}

	SolverProtocol::Packet_DataFromSolver_SolverSolution_SingleMarks pktSingleMark;
	if(autoAlt) {
		pktSingleMark =  pkt.datafromsolver().solution_automatic_altitude().singlemarks();
	} else {
		pktSingleMark =  pkt.datafromsolver().solution_manual_altitude().singlemarks();
	}


    for(int i = 0; i < pktSingleMark.singlemark_size(); i++ ) {
        SolverProtocol::Packet_DataFromSolver_SolverSolution_SingleMarks_SingleMark pktSingle =  pktSingleMark.singlemark( i );

        UAVPositionDataEnemy dataStruct;

        dataStruct.name = tr("%1_single").arg( (int)pktSingleMark.central_frequency() );
        dataStruct.altitude = pktSingle.coordinates().alt();
        dataStruct.altitudeStdDev = pktSingle.coordinates_acc().alt_acc();
        dataStruct.speed = 0;
        dataStruct.course = 0;
        dataStruct.state = 0;
        dataStruct.frequency = pktSingleMark.central_frequency();
        dataStruct.time = QDateTime::fromMSecsSinceEpoch(pktSingleMark.datetime());
        dataStruct.latLon = QPointF( pktSingle.coordinates().lat(), pktSingle.coordinates().lon() );
        dataStruct.latLonStdDev = QPointF( pktSingle.coordinates_acc().lat_acc(),
                                           pktSingle.coordinates_acc().lon_acc() );
        dataStruct.sourceType = 0;

        dataStruct.course = 0;

        retList.append(dataStruct);
    }

    return retList;

}

QList<UAVPositionDataEnemy> DbUavManager::getUavListTrajectoryFromProto(SolverProtocol::
                             Packet_DataFromSolver_SolverSolution_Trajectory pktTrajectory)
{
    QList<UAVPositionDataEnemy> retList = QList<UAVPositionDataEnemy>();

    for(int i = 0; i < pktTrajectory.motionestimate_size(); i++ ) {

        UAVPositionDataEnemy dataStruct;
        SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory_MotionEstimate pktSingle = pktTrajectory.motionestimate(i);

        dataStruct.name = QString::fromStdString(pktTrajectory.targetid());
        dataStruct.altitude = pktSingle.coordinates().alt();
        dataStruct.altitudeStdDev = pktSingle.coordinates_acc().alt_acc();
        dataStruct.speed = pktSingle.targetspeed();
        dataStruct.course = pktSingle.relativebearing();
        dataStruct.state = (int)pktSingle.state();
        dataStruct.frequency = pktTrajectory.central_frequency();
        QString test = QString::fromStdString(pktTrajectory.targetid());
        Q_UNUSED(test);
        quint64 t = pktSingle.datetime();
        dataStruct.time = QDateTime::fromMSecsSinceEpoch(t);
        dataStruct.latLon = QPointF( pktSingle.coordinates().lat(), pktSingle.coordinates().lon() );
        dataStruct.latLonStdDev = QPointF( pktSingle.coordinates_acc().lat_acc(),
                                           pktSingle.coordinates_acc().lon_acc() );
        dataStruct.sourceType = 0;

        retList.append(dataStruct);
    }

    return retList;
}

void DbUavManager::addUavInfoToDb(const UAVPositionData& positionData, const QString& role,
								  const QString& uavType, const QString& status,
								  const QString& deviceType, const QString& sourceType,
								  bool actual,
								  const QVector<QPointF>& tail,
								  const QVector<QPointF>& tailStdDev, const QVector<QDateTime> &tailTime)
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
		uav.freqId = positionData.frequency;

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

	//SOURCE
	int sourceTypeId = getSourceTypeByName(sourceType);

	if (sourceTypeId < 0){
		SourceType sourceTypeStruct;
		sourceTypeStruct.name = sourceType;
		sourceTypeId = addSourceType(sourceTypeStruct);
	}

	int sourceId = getSourceId(positionData.sourceType, sourceTypeId);

	if (sourceId < 0){
		Source source;
		source.sourceTypeId = sourceTypeId;
		source.sourceId = positionData.sourceType;

		addSource(source);
	}

	UavInfo uavInfo;
    uavInfo.name = positionData.name;
	uavInfo.uavId = uavId; // FK
	uavInfo.device = deviceUnknownId; // FK
	uavInfo.lat = positionData.latitude;
	uavInfo.lon = positionData.longitude;
	uavInfo.latStddev = positionData.latitudeStddev;
	uavInfo.lonStddev = positionData.longitudeStddev;
	uavInfo.role = role;

	if(positionData.altitude != positionData.altitude)
	{
		uavInfo.alt = 0;
	}else
	if(positionData.altitude < 100 || positionData.altitude > 10000)
	{
		uavInfo.alt = 0;
	}else
	{
		uavInfo.alt = positionData.altitude;
	}
	uavInfo.speed = positionData.speed;
	if(positionData.course != positionData.course)
	{
		uavInfo.yaw = 0;
	}else
	if(positionData.course < 0 || positionData.course > 359)
	{
		uavInfo.yaw = 0;
	}
	else {
		uavInfo.yaw = positionData.course;
	}
	uavInfo.restTime = QTime(1, 0);
    uavInfo.statusId = status.toUInt(); // FK
	uavInfo.dateTime = positionData.dateTime;

	if(sourceId < 0) {
		uavInfo.source = positionData.sourceType;
	} else {
		uavInfo.source = sourceId;
	}

	addUavInfo(uavInfo, actual, tail, tailStdDev, tailTime);
}

QString DbUavManager::getEnemySourceTypeName(uint sourceType)
{
	QString name;

	switch( sourceType ) {
		case UAV_SOLVER_MANUAL_SOURCE:
			name = "Solver Manual";
			break;

		case UAV_SOLVER_AUTO_SOURCE:
			name = "Solver Auto";
			break;

		case UAV_SOLVER_SINGLE_1_SOURCE:
			name = "Solver Single 1";
			break;

		case UAV_SOLVER_SINGLE_2_SOURCE:
			name = "Solver Single 2";
			break;

		default:
			name = "UnknownSourceType";
	}

	return name;
}

void DbUavManager::sendEnemyUavPoints(const QByteArray& data, uint sourceType)
{
	QDataStream ds(data);

	QList<UAVPositionDataEnemy> uavList;
	ds >> uavList;

	sendEnemyUavPoints(uavList, sourceType);
}

void DbUavManager::sendEnemyUavPoints( const QList<UAVPositionDataEnemy>& list, uint sourceType ) {

	QList<UAVPositionDataEnemy> uavList = list;
	QVector<QPointF> tail;
	QVector<QPointF> tailStdDev;
	QVector<QDateTime> tailTime;

	bool isActual; // is it record displays information about BPLA in current time?
	QVector<QPointF> emptyVector;
	QVector<QDateTime> emptyVectorTime;

	bool singleSourceType = sourceType == UAV_SOLVER_SINGLE_1_SOURCE
			|| sourceType == UAV_SOLVER_SINGLE_2_SOURCE;

//	if ( !m_dbController->beginTransaction() ) {
//		log_warning( "Database transaction was not started!" );  //Errors here (
//	}

	for ( int i = 0; i < uavList.length(); i++ ) {
		UAVPositionDataEnemy &uav = uavList[i];
		uav.sourceType = sourceType == UAV_SOLVER_SINGLE_1_SOURCE && i % 2 == 1
						? UAV_SOLVER_SINGLE_2_SOURCE : sourceType;

		tail << uav.latLon;
		tailStdDev << uav.latLonStdDev;
		tailTime << uav.time;

		isActual = i + 1 == uavList.length() || singleSourceType;

		if(!isActual) {
			continue;
		}

		addUavInfoToDb( uav, ENEMY_UAV_ROLE,
                        "UnknownUavType", QString::number(uav.state), "UnknownDeviceType",
						getEnemySourceTypeName(uav.sourceType),
						isActual,
						isActual && !singleSourceType ? tail : emptyVector,
						isActual && !singleSourceType ? tailStdDev : emptyVector,
						isActual && !singleSourceType ? tailTime : emptyVectorTime );

	}

	//m_dbController->commit();
}

void DbUavManager::addUavInfoToDb(const UAVPositionDataEnemy& positionDataEnemy, const QString &role,
								  const QString &uavType, const QString &status, const QString &deviceType,
								  const QString &sourceType,
								  bool actual,
								  const QVector<QPointF> &tail,
								  const QVector<QPointF> &tailStdDev, const QVector<QDateTime> &tailTime)
{
	UAVPositionData positionData;
	positionData.altitude = positionDataEnemy.altitude;
	positionData.course = positionDataEnemy.course;
	positionData.speed = positionDataEnemy.speed;
	positionData.state = positionDataEnemy.state;

	// готово
	positionData.latitude = positionDataEnemy.latLon.x();
	positionData.longitude = positionDataEnemy.latLon.y();
	positionData.latitudeStddev = positionDataEnemy.latLonStdDev.x();
	positionData.longitudeStddev = positionDataEnemy.latLonStdDev.y();

	if(positionDataEnemy.frequency < 0) {

		positionData.boardID = ENEMY_UAV_ID_OFFSET;
	}
	else {
		positionData.boardID = positionDataEnemy.frequency;
	}

	positionData.frequency = positionDataEnemy.frequency;
	positionData.sourceType = positionDataEnemy.sourceType;
    positionData.name = positionDataEnemy.name;

	// transform time to dateTime
    QDateTime time = positionDataEnemy.time;
    positionData.dateTime = time;

	addUavInfoToDb(positionData, role, uavType, status, deviceType, sourceType, actual, tail, tailStdDev, tailTime);
}

void DbUavManager::timeoutSlot(const QString& key)
{
	QMutexLocker deleteLocker(&m_deleteMutex);

    Uav uav = m_knownUavsList.take(key);
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
