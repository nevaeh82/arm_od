#include "DbUavController.h"

DbUavController::DbUavController(QObject *parent) :
	DbControllerBase("DBBLACONNECTION", "QMYSQL", parent)
{

}

DbUavController::~DbUavController()
{
	m_db.close();
}

int DbUavController::addUav(const Uav& uav)
{
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("INSERT INTO uav (uavId, ip, uavTypeId, name, freqId) VALUES(:uavId, :ip, :uavTypeId, :name, :freqId);");

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":uavId", uav.uavId);
	query.bindValue(":ip", uav.ip);
	query.bindValue(":uavTypeId", uav.uavTypeId);
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
	Uav uav;
	uav.id = INVALID_INDEX;

	if(!m_db.isOpen()){
		return uav;
	}

	QSqlQuery query(m_db);
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
		uav.name = query.record().value(4).toString();
	}

	return uav;
}

Uav DbUavController::getUav(const uint id)
{
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
		uav.name = query.record().value(4).toString();
	}

	return uav;
}

int DbUavController::addUavInfo(const UavInfo& info)
{
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO info (uavId, device, latitude, longitude, altitude, speed, yaw, restTime, statusTypeId, datetime)")
								   + QString("VALUES(:uavId, :device, :lat, :lon, :alt, :speed, :yaw, :restTime, :statusId, :dateTime);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":uavId", info.uavId);
	query.bindValue(":device", info.device);
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
	return INVALID_INDEX;
}

int DbUavController::addDevice(const Devices& device)
{
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

int DbUavController::addUavMission(const UavMission& mission)
{
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

int DbUavController::addTarget(const Target& target)
{
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO target (ip, port, targetTypeId)")
								   + QString("VALUES(:ip, :port, :targetTypeId);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":ip", target.ip);
	query.bindValue(":port", target.port);
	query.bindValue(":targetTypeId", target.type);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

bool DbUavController::getTargetsByType(const uint targetTypeId, QList<Target> &targetsRecords)
{
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

		target.id = query.value(0).toUInt();
		target.ip = query.value(1).toString();
		target.port = query.value(2).toUInt();
		target.type = query.value(3).toUInt();

		targetsRecords.append(target);
	}

	return true;
}

int DbUavController::addTargetType(const TargetType& targetType)
{
	return addDictionaryRecord("targettypes", targetType.name);
}

int DbUavController::getTargetTypeByName(const QString& name)
{
	return getDictionaryRecord("targettypes", name);
}

int DbUavController::addUavType(const UavType &uavType)
{
	return addDictionaryRecord("uavtypes", uavType.name);
}

int DbUavController::getUavTypeByName(const QString& name)
{
	return getDictionaryRecord("uavtypes", name);
}

int DbUavController::addDeviceType(const DeviceType& device)
{
	return addDictionaryRecord("devicetypes", device.name);
}

int DbUavController::getDeviceTypeByName(const QString& name)
{
	return getDictionaryRecord("devicetypes", name);
}

int DbUavController::addStatus(const Status& status)
{
	return addDictionaryRecord("statustypes", status.status);
}

int DbUavController::getStatusByName(const QString& name)
{
	return getDictionaryRecord("statustypes", name);
}

int DbUavController::addUavRole(const UavRole& uavRole)
{
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

UavRole DbUavController::getUavRoleByName(const QString& name)
{
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
