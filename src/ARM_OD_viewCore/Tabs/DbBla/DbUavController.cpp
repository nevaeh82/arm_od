#include "Tabs/DbBla/DbUavController.h"

DbUavController::DbUavController(QObject *parent)
	: DbControllerBase("DBBLACONNECTION", "QMYSQL", parent)
	, m_uavHistory( NULL )
{

}

DbUavController::DbUavController(QString connectionName, QString dbType, QObject *parent)
	: DbControllerBase(connectionName, dbType, parent)
	, m_uavHistory( NULL )
{

}


DbUavController::~DbUavController()
{
	m_db.close();
}

void DbUavController::disconnectFromDb()
{
	if (m_db.isOpen()) {
		m_db.close();
	}
}

int DbUavController::addUav(const Uav& uav)
{
	QMutexLocker locker(&m_addGetUavMutex);

	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("INSERT INTO uav (uavId, ip, uavTypeId, roleId, name, freqId) VALUES(:uavId, :ip, :uavTypeId, :roleId, :name, :freqId);");

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	QString uavRole = getUavRole(uav.roleId).code;

	if (uavRole == ENEMY_UAV_ROLE){
		int enemyUavId = /*getUavsCountByRole(ENEMY_UAV_ROLE) +*/ ENEMY_UAV_ID_OFFSET;
		query.bindValue(":uavId", enemyUavId);
	} else {
		query.bindValue(":uavId", uav.uavId);
	}

	query.bindValue(":ip", uav.ip);
	query.bindValue(":uavTypeId", uav.uavTypeId);
	query.bindValue(":roleId", uav.roleId);
	query.bindValue(":name", uav.name);
	query.bindValue(":freqId", uav.freqId);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

Uav DbUavController::getUavByUavId(const uint uavId)
{
	QMutexLocker locker(&m_addGetUavMutex);

	Uav uav;
	uav.id = INVALID_INDEX;

	if(!m_db.isOpen()){
		return uav;
	}

	QSqlQuery query(m_db);
	m_db.transaction();
	bool succeeded = query.prepare("SELECT * FROM uav WHERE uavId = :id;");

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return uav;
	}

	query.bindValue(":id", uavId);


	succeeded = query.exec();
	QString queryTxt = query.executedQuery();

	if(!query.next()) {
		return uav;
	}

	if (succeeded){
		uav.id = query.record().value(0).toUInt();
		uav.uavId = query.record().value(1).toUInt();
		uav.ip = query.record().value(2).toString();
		uav.uavTypeId = query.record().value(3).toUInt();
		uav.roleId = query.record().value(4).toUInt();
		uav.name = query.record().value(5).toString();
		uav.freqId = query.record().value(6).toUInt();
	}

	return uav;
}

Uav DbUavController::getUav(const uint id)
{
	QMutexLocker locker(&m_addGetUavMutex);

	Uav uav;
	uav.id = INVALID_INDEX;

	if(!m_db.isOpen()){
		return uav;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT * FROM uav WHERE id = :id;");

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return uav;
	}

	query.bindValue(":id", id);

	succeeded = query.exec();
	QString queryTxt = query.executedQuery();

	if(!query.next()) {
		return uav;
	}

	if (succeeded){
		uav.id = query.record().value(0).toUInt();
		uav.uavId = query.record().value(1).toUInt();
		uav.ip = query.record().value(2).toString();
		uav.uavTypeId = query.record().value(3).toUInt();
		uav.roleId = query.record().value(4).toUInt();
		uav.name = query.record().value(5).toString();
		uav.freqId = query.record().value(6).toUInt();
	}

	return uav;
}

bool DbUavController::getUavsByRole(const QString &role, QList<Uav> &uavs)
{
	QMutexLocker locker(&m_addGetUavMutex);

	if(!m_db.isOpen()){
		return false;
	}

	uavs.clear();

	QSqlQuery query(m_db);

	int roleId = getUavRoleByCode(role).id;

	if (INVALID_INDEX == roleId){
		return false;
	}

	bool succeeded = query.prepare("SELECT * FROM uav WHERE roleId = :roleId;");

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return false;
	}

	query.bindValue(":roleId", roleId);

	succeeded = query.exec();

	if (!succeeded){
		return false;
	}

	while (query.next()){
		Uav uav;
		uav.id = query.record().value(0).toUInt();
		uav.uavId = query.record().value(1).toUInt();
		uav.ip = query.record().value(2).toString();
		uav.uavTypeId = query.record().value(3).toUInt();
		uav.roleId = query.record().value(4).toUInt();
		uav.name = query.record().value(5).toString();
		uav.freqId = query.record().value(6).toUInt();
		uavs.append(uav);
	}

	return true;
}

int DbUavController::getUavsCountByRole(const QString &role)
{
	QMutexLocker locker(&m_addGetUavMutex);

	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT COUNT(*) AS CNT FROM UAV WHERE roleId = (SELECT ID FROM UAVROLES WHERE code = :roleCode);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":roleCode", role);

	succeeded = query.exec();

	if (!succeeded){
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
		return INVALID_INDEX;
	}

	if(!query.next()) {
		return INVALID_INDEX;
	}


	return query.record().value(0).toUInt();
}

int DbUavController::addUavInfo(const UavInfo& info)
{
	QMutexLocker locker(&m_addGetUavInfoMutex);

	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO info (uavId, device, source, latitude, longitude, altitude, speed, yaw, restTime, statusTypeId, datetime)")
								   + QString("VALUES(:uavId, :device, :source, :lat, :lon, :alt, :speed, :yaw, :restTime, :statusId, :dateTime);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":uavId", info.uavId);
	query.bindValue(":device", info.device);
	query.bindValue(":source", info.source);
	query.bindValue(":lat", info.lat);
	query.bindValue(":lon", info.lon);
	query.bindValue(":alt", info.alt);
	query.bindValue(":speed", info.speed);
	query.bindValue(":yaw", info.yaw);
	query.bindValue(":restTime", info.restTime.toString("hh:mm:ss"));
	query.bindValue(":statusId", info.statusId);
	query.bindValue(":dateTime", info.dateTime.toString("yyyy-MM-dd hh:mm:ss"));

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

int DbUavController::getUavInfoByUavId(const uint)
{
	QMutexLocker locker(&m_addGetUavInfoMutex);

	return INVALID_INDEX;
}

IUavHistory* DbUavController::getUavHistory()
{
	if( m_uavHistory == NULL ) {
		m_uavHistory = new UavHistory( m_db, this );
		m_uavHistory->moveToThread( this->thread() );
	}

	return m_uavHistory;
}

int DbUavController::addDevice(const Devices& device)
{
	QMutexLocker locker(&m_addGetDeviceMutex);

	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO devices (deviceTypeId, port, uavId)")
								   + QString("VALUES(:deviceTypeId, :port, :uavId);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":deviceTypeId", device.deviceId);
	query.bindValue(":port", device.port);
	query.bindValue(":uavId", device.uavId);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

bool DbUavController::getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords)
{
	QMutexLocker locker(&m_addGetDeviceMutex);

	if(!m_db.isOpen()){
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT * FROM devices WHERE deviceTypeId = :deviceTypeId;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return false;
	}

	query.bindValue(":deviceTypeId", deviceTypeId);

	succeeded = query.exec();

	if (!succeeded){
		return false;
	}

	while (query.next()){
		Devices device;
		device.id = query.value(0).toUInt();
		device.deviceId = query.value(1).toUInt();
		device.port = query.value(2).toUInt();
		device.uavId = query.value(3).toUInt();
		devicesRecords.append(device);
	}

	return true;
}

int DbUavController::addSource(const Sources& source)
{
	QMutexLocker locker(&m_addGetDeviceMutex);

	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO sources (sourceTypeId, sourceId)")
								   + QString("VALUES(:sourceTypeId, :sourceId);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":sourceTypeId", source.sourceTypeId);
	query.bindValue(":sourceId", source.sourceId);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

bool DbUavController::getSourceByType(const uint sourceTypeId, QList<Sources> &sourcesRecords)
{
	QMutexLocker locker(&m_addGetDeviceMutex);

	if(!m_db.isOpen()){
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT * FROM sources WHERE sourceTypeId = :sourceTypeId;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return false;
	}

	query.bindValue(":sourceTypeId", sourceTypeId);

	succeeded = query.exec();

	if (!succeeded){
		return false;
	}

	while (query.next()){
		Sources source;
		source.id = query.value(0).toUInt();
		source.sourceTypeId = query.value(1).toUInt();
		source.sourceId = query.value(2).toUInt();
		sourcesRecords.append(source);
	}

	return true;
}

Sources DbUavController::getSource(const uint sourceId)
{
	QMutexLocker locker(&m_addGetDeviceMutex);

	Sources source;
	source.id = -1;

	if(!m_db.isOpen()){
		return source;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT * FROM sources WHERE id = :sourceId;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return source;
	}

	query.bindValue(":sourceId", sourceId);

	succeeded = query.exec();

	if (!succeeded){
		return source;
	}

	while (query.next()){
		source.id = query.value(0).toUInt();
		source.sourceTypeId = query.value(1).toUInt();
		source.sourceId = query.value(2).toUInt();
	}

	return source;
}

int DbUavController::addUavMission(const UavMission& mission)
{
	QMutexLocker locker(&m_addGetUavMissionMutex);

	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO uavmission (uavId, targetID, regionCenterLatitude, regionCenterLongitude, regionCenterAltitude, regionRadius, timeToTarget)")
								   + QString("VALUES(:uavId, :targetID, :rcLat, :rcLon, :rcAlt, :rRad, :timeToTarget);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":uavId", mission.uavId);
	query.bindValue(":targetID", mission.targetId);
	query.bindValue(":rcLat", mission.regionCenterLat);
	query.bindValue(":rcLon", mission.regionCenterLon);
	query.bindValue(":rcAlt", mission.regionCenterAltitude);
	query.bindValue(":rRad", mission.regionRadius);
	query.bindValue(":timeToTarget", mission.timeToTarget.toString("hh:mm:ss"));

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

bool DbUavController::getUavMissionsByUavId(const uint uavId, QList<UavMission> &missionsRecords)
{
	QMutexLocker locker(&m_addGetUavMissionMutex);

	if(!m_db.isOpen()){
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT * FROM uavmission WHERE uavId = :uavId;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return false;
	}

	query.bindValue(":uavId", uavId);

	succeeded = query.exec();

	if (!succeeded){
		return false;
	}

	while (query.next()){
		UavMission mission;

		mission.id = query.value(0).toUInt();
		mission.uavId = query.value(1).toUInt();
		mission.targetId = query.value(2).toUInt();
		mission.regionCenterLat = query.value(3).toDouble();
		mission.regionCenterLon = query.value(4).toDouble();
		mission.regionCenterAltitude = query.value(5).toDouble();
		mission.regionRadius = query.value(6).toDouble();
		mission.timeToTarget = QTime::fromString(query.value(7).toString());

		missionsRecords.append(mission);
	}

	return true;
}

bool DbUavController::deleteUavMissionsByUavId(const uint uavId)
{
	QMutexLocker locker(&m_addGetTargetMutex);

	if(!m_db.isOpen()){
		return false;
	}

	QList<UavMission> missionList;
	if(!getUavMissionsByUavId(uavId, missionList)) {
		return false;
	}

	foreach (UavMission mission, missionList) {

		if (INVALID_INDEX == mission.id){
			return false;
		}

		QSqlQuery query(m_db);
		bool succeeded = query.prepare(QString("DELETE FROM uavmission WHERE uavID = :uavId;"));

		if (!succeeded) {
			QString er = query.lastError().text();
			log_debug("SQL is wrong! " + er);
			return false;
		}

		query.bindValue(":uavId", uavId);

		succeeded = query.exec();

		if (!succeeded){
			QString er = query.lastError().text();
			log_debug("SQL is wrong! " + er);
			m_db.commit();
			return false;
		}

		m_db.commit();
	}

	return true;
}

int DbUavController::addTarget(const Target& target)
{
	QMutexLocker locker(&m_addGetTargetMutex);

	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	m_db.transaction();

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO target (uavID, ip, port, targetTypeId)")
								   + QString("VALUES(:uavId, :ip, :port, :targetTypeId);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		m_db.commit();
		return INVALID_INDEX;
	}

	query.bindValue(":uavId", target.uavId);
	query.bindValue(":ip", target.ip);
	query.bindValue(":port", target.port);
	query.bindValue(":targetTypeId", target.type);

	succeeded = query.exec();

	if (succeeded){
		m_db.commit();
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	m_db.commit();
	return INVALID_INDEX;
}

bool DbUavController::getTargetsByType(const uint targetTypeId, QList<Target> &targetsRecords)
{
	QMutexLocker locker(&m_addGetTargetMutex);

	if(!m_db.isOpen()){
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT * FROM target WHERE targetTypeId = :targetTypeId;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return false;
	}

	query.bindValue(":targetTypeId", targetTypeId);

	succeeded = query.exec();

	if (!succeeded){
		return false;
	}

	while (query.next()){
		Target target;

		target.id = query.value(0).toInt();
		target.uavId = query.value(1).toInt();
		target.ip = query.value(2).toString();
		target.port = query.value(3).toInt();
		target.type = query.value(4).toInt();

		targetsRecords.append(target);
	}

	return true;
}

bool DbUavController::getTargetsByUavId(const uint uavId, QList<Target> &targetsRecords)
{
	QMutexLocker locker(&m_addGetTargetMutex);

	if(!m_db.isOpen()){
		return false;
	}

	Uav uav = getUavByUavId(uavId);

	if (INVALID_INDEX == uav.id){
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT * FROM target WHERE uavId = :uavId;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return false;
	}

	query.bindValue(":uavId", uav.id);

	succeeded = query.exec();

	if (!succeeded){
		return false;
	}

	while (query.next()){
		Target target;

		target.id = query.value(0).toInt();
		target.uavId = query.value(1).toInt();
		target.ip = query.value(2).toString();
		target.port = query.value(3).toInt();
		target.type = query.value(4).toInt();

		targetsRecords.append(target);
	}

	return true;
}

bool DbUavController::getTargetsByMission(const uint missionId, QList<Target>& targetsRecords)
{
	QMutexLocker locker(&m_addGetTargetMutex);

	if(!m_db.isOpen()){
		return false;
	}

	QSqlQuery query0(m_db);
	bool succeeded = query0.prepare(QString("SELECT targetID FROM uavmission WHERE id = :missionID"));
	if (!succeeded) {
		QString er = query0.lastError().text();
		log_debug("SQL is wrong! " + er);
		return false;
	}

	query0.bindValue(":missionID", missionId);

	succeeded = query0.exec();

	if (!succeeded){
		return false;
	}

	int targetID;
	while (query0.next()){
		targetID = query0.value(0).toInt();
		break;
	}

	QSqlQuery query(m_db);
	succeeded = query.prepare(QString("SELECT * FROM target WHERE id = :targetID;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return false;
	}

	query.bindValue(":targetID", targetID);

	succeeded = query.exec();

	if (!succeeded){
		return false;
	}

	while (query.next()){
		Target target;

		target.id = query.value(0).toInt();
		target.uavId = query.value(1).toInt();
		target.ip = query.value(2).toString();
		target.port = query.value(3).toInt();
		target.type = query.value(4).toInt();

		targetsRecords.append(target);
	}

	return true;
}

bool DbUavController::deleteTargetsByUavId(const uint uavId)
{
	QMutexLocker locker(&m_addGetTargetMutex);

	if(!m_db.isOpen()){
		return false;
	}

	m_db.transaction();

	Uav uav = getUavByUavId(uavId);

	if (INVALID_INDEX == uav.id){
		m_db.commit();
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("DELETE FROM target WHERE uavID = :uavId;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		m_db.commit();
		return false;
	}

	query.bindValue(":uavId", uav.id);

	succeeded = query.exec();

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		m_db.commit();
		return false;
	}

	m_db.commit();
	return true;
}

bool DbUavController::deleteTargetsById(const uint id)
{
	QMutexLocker locker(&m_addGetTargetMutex);

	if(!m_db.isOpen()){
		return false;
	}

	m_db.transaction();

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("DELETE FROM target WHERE id = :id;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		m_db.commit();
		return false;
	}

	query.bindValue(":id", id);

	succeeded = query.exec();

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		m_db.commit();
		return false;
	}

	m_db.commit();
	return true;
}

int DbUavController::addTargetType(const TargetType& targetType)
{
	QMutexLocker locker(&m_addGetTargetTypeMutex);
	return addDictionaryRecord("targettypes", targetType.name);
}

int DbUavController::getTargetTypeByName(const QString& name)
{
	QMutexLocker locker(&m_addGetTargetTypeMutex);
	return getDictionaryRecord("targettypes", name);
}

int DbUavController::addUavType(const UavType &uavType)
{
	QMutexLocker locker(&m_addGetUavTypeMutex);
	return addDictionaryRecord("uavtypes", uavType.name);
}

int DbUavController::getUavTypeByName(const QString& name)
{
	QMutexLocker locker(&m_addGetUavTypeMutex);
	return getDictionaryRecord("uavtypes", name);
}

int DbUavController::addDeviceType(const DeviceType& device)
{
	QMutexLocker locker(&m_addGetDeviceTypeMutex);
	return addDictionaryRecord("devicetypes", device.name);
}

int DbUavController::getDeviceTypeByName(const QString& name)
{
	QMutexLocker locker(&m_addGetDeviceTypeMutex);
	return getDictionaryRecord("devicetypes", name);
}

int DbUavController::addSourceType(const SourceType& source)
{
	QMutexLocker locker(&m_addGetDeviceTypeMutex);
	return addDictionaryRecord("sourcetypes", source.name);
}

int DbUavController::getSourceTypeByName(const QString& name)
{
	QMutexLocker locker(&m_addGetDeviceTypeMutex);
	return getDictionaryRecord("sourcetypes", name);
}

int DbUavController::addStatus(const Status& status)
{
	QMutexLocker locker(&m_addGetStatusMutex);
	return addDictionaryRecord("statustypes", status.status);
}

int DbUavController::getStatusByName(const QString& name)
{
	QMutexLocker locker(&m_addGetStatusMutex);
	return getDictionaryRecord("statustypes", name);
}

int DbUavController::addUavRole(const UavRole& uavRole)
{
	QMutexLocker locker(&m_addGetUavRoleMutex);
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO uavroles (code, name)")
								   + QString("VALUES(:code, :name);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":code", uavRole.code);
	query.bindValue(":name", uavRole.name);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

UavRole DbUavController::getUavRole(const uint roleId)
{
	QMutexLocker locker(&m_addGetUavRoleMutex);

	UavRole uavRole;
	uavRole.id = INVALID_INDEX;

	if(!m_db.isOpen()){
		return uavRole;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT * FROM uavroles WHERE id = :roleId;");

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return uavRole;
	}

	query.bindValue(":roleId", roleId);

	succeeded = query.exec();

	if(!query.next()) {
		return uavRole;
	}

	if (succeeded){
		uavRole.id = query.record().value(0).toUInt();
		uavRole.code = query.record().value(1).toString();
		uavRole.name = query.record().value(2).toString();
	}

	return uavRole;
}

UavRole DbUavController::getUavRoleByName(const QString& name)
{
	QMutexLocker locker(&m_addGetUavRoleMutex);

	UavRole uavRole;
	uavRole.id = INVALID_INDEX;

	if(!m_db.isOpen()){
		return uavRole;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT id FROM uavroles WHERE name = :name;");

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return uavRole;
	}

	query.bindValue(":name", name);

	succeeded = query.exec();

	if(!query.next()) {
		return uavRole;
	}

	if (succeeded){
		uavRole.id = query.record().value(0).toUInt();
		uavRole.code = query.record().value(1).toString();
		uavRole.name = query.record().value(2).toString();
		return uavRole;
	}

	return uavRole;
}

UavRole DbUavController::getUavRoleByCode(const QString& code)
{
	QMutexLocker locker(&m_addGetUavRoleMutex);

	UavRole uavRole;
	uavRole.id = INVALID_INDEX;

	if(!m_db.isOpen()){
		return uavRole;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT id FROM uavroles WHERE code = :code;");

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return uavRole;
	}

	query.bindValue(":code", code);

	succeeded = query.exec();

	if(!query.next()) {
		return uavRole;
	}

	if (succeeded){
		uavRole.id = query.record().value(0).toUInt();
		uavRole.code = query.record().value(1).toString();
		uavRole.name = query.record().value(2).toString();
		return uavRole;
	}

	return uavRole;
}

int DbUavController::addDictionaryRecord(const QString& dictionary, const QString& name)
{
	QMutexLocker locker(&m_addGetDictionaryMutex);

	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO %1 (name) VALUES(:name);").arg(dictionary));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":name", name);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

int DbUavController::getDictionaryRecord(const QString& dictionary, const QString& name)
{
	QMutexLocker locker(&m_addGetDictionaryMutex);

	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT id FROM %1 WHERE name = :name;").arg(dictionary));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":dictionaryTable", dictionary);
	query.bindValue(":name", name);

	succeeded = query.exec();

	if(!query.next()) {
		return INVALID_INDEX;
	}

	if (succeeded){
		return query.record().value(0).toUInt();
	}

	return INVALID_INDEX;
}
