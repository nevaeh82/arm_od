#include "DbBlaController.h"

DbBlaController::DbBlaController(QObject *parent) :
	DbControllerBase("DBBLACONNECTION", "QMYSQL", parent)
{

}

DbBlaController::~DbBlaController()
{
	m_db.close();
}

int DbBlaController::addBla(const Bla& bla)
{
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("INSERT INTO bla (idBla, ip, type, name) VALUES(:blaId, :ip, :type, :name);");

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":blaId", bla.blaId);
	query.bindValue(":ip", bla.ip);
	query.bindValue(":type", bla.type);
	query.bindValue(":name", bla.name);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

int DbBlaController::getBlaByBlaId(const uint blaId)
{
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT * FROM bla WHERE idBLA = :id;");

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":id", blaId);


	succeeded = query.exec();
	QString queryTxt = query.executedQuery();

	if(!query.next()) {
		return INVALID_INDEX;
	}

	if (succeeded){
		return query.record().value(0).toUInt();
	}

	return INVALID_INDEX;
}

int DbBlaController::addBlaInfo(const BlaInfo& info)
{
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO info (BLA_id, device, latitude, longitude, altitude, speed, yaw, restTime, statusID, datetime)")
								   + QString("VALUES(:blaId, :device, :lat, :lon, :alt, :speed, :yaw, :restTime, :statusId, :dateTime);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":blaId", info.blaId);
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

int DbBlaController::getBlaInfoByBlaId(const uint blaId)
{
	return INVALID_INDEX;
}

int DbBlaController::addDevice(const Devices& device)
{
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO devices (deviceID, port, BLAID)")
								   + QString("VALUES(:deviceID, :port, :BLAID);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":deviceID", device.deviceId);
	query.bindValue(":port", device.port);
	query.bindValue(":BLAID", device.blaId);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

bool DbBlaController::getDevicesByType(const uint deviceTypeId, QList<Devices>& devicesRecords)
{
	if(!m_db.isOpen()){
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT * FROM devices WHERE deviceID = :deviceTypeId;"));

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
		device.blaId = query.value(3).toUInt();
		devicesRecords.append(device);
	}

	return true;
}

int DbBlaController::addBlaMission(const BlaMission& mission)
{
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO blamission (BLAid, targetID, regionCenterLatitude, regionCenterLongitude, regionCenterAtitude, regionRadius, timeToTarget)")
								   + QString("VALUES(:BLAid, :targetID, :rcLat, :rcLon, :rcAlt, :rRad, :timeToTarget);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":BLAid", mission.blaId);
	query.bindValue(":targetID", mission.targetId);
	query.bindValue(":rcLat", mission.regionCenterLat);
	query.bindValue(":rcLon", mission.regionCenterLon);
	query.bindValue(":rcAlt", mission.regionCenterAtitude);
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

bool DbBlaController::getBlaMissionsByBlaId(const uint blaId, QList<BlaMission> &missionsRecords)
{
	if(!m_db.isOpen()){
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT * FROM blamission WHERE BLAid = :BLAid;"));

	if (!succeeded) {
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return false;
	}

	query.bindValue(":BLAid", blaId);

	succeeded = query.exec();

	if (!succeeded){
		return false;
	}

	while (query.next()){
		BlaMission mission;

		mission.id = query.value(0).toUInt();
		mission.blaId = query.value(1).toUInt();
		mission.targetId = query.value(2).toUInt();
		mission.regionCenterLat = query.value(3).toDouble();
		mission.regionCenterLon = query.value(4).toDouble();
		mission.regionCenterAtitude = query.value(5).toDouble();
		mission.regionRadius = query.value(6).toDouble();
		mission.timeToTarget = QTime::fromString(query.value(7).toString());

		missionsRecords.append(mission);
	}

	return true;
}

int DbBlaController::addTarget(const Target& target)
{
	if(!m_db.isOpen()){
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("INSERT INTO target (ip, port, targetID, type)")
								   + QString("VALUES(:ip, :port, :targetID, :type);"));

	if (!succeeded){
		QString er = query.lastError().text();
		log_debug("SQL is wrong! " + er);
		return INVALID_INDEX;
	}

	query.bindValue(":ip", target.ip);
	query.bindValue(":port", target.port);
	query.bindValue(":targetID", target.targetId);
	query.bindValue(":type", target.type);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	} else {
		QString er = query.lastError().databaseText() + "\n" + query.lastError().driverText();
		log_debug("SQL query is wrong! " + er);
	}

	return INVALID_INDEX;
}

bool DbBlaController::getTargetsByType(const uint targetTypeId, QList<Target> &targetsRecords)
{
	if(!m_db.isOpen()){
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(QString("SELECT * FROM target WHERE type = :targetTypeId;"));

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
		target.targetId = query.value(3).toUInt();
		target.type = query.value(4).toUInt();

		targetsRecords.append(target);
	}

	return true;
}

int DbBlaController::addTargetType(const TargetType& targetType)
{
	return addDictionaryRecord("targettypedictionary", targetType.name);
}

int DbBlaController::getTargetTypeByName(const QString& name)
{
	return getDictionaryRecord("targettypedictionary", name);
}

int DbBlaController::addBlaType(const BlaType& blaType)
{
	return addDictionaryRecord("typebladictionary", blaType.name);
}

int DbBlaController::getBlaTypeByName(const QString& name)
{
	return getDictionaryRecord("typebladictionary", name);
}

int DbBlaController::addDeviceType(const DeviceType& device)
{
	return addDictionaryRecord("devicedictionary", device.name);
}

int DbBlaController::getDeviceTypeByName(const QString& name)
{
	return getDictionaryRecord("devicedictionary", name);
}

int DbBlaController::addStatus(const Status& status)
{
	return addDictionaryRecord("statusdictionary", status.status);
}

int DbBlaController::getStatusByName(const QString& name)
{
	return getDictionaryRecord("statusdictionary", name);
}

int DbBlaController::addDictionaryRecord(const QString& dictionary, const QString& name)
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

int DbBlaController::getDictionaryRecord(const QString& dictionary, const QString& name)
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
