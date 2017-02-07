#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QEventLoop>
#include <QThread>

#include <Logger/Logger.h>

#include "UavHistory.h"
#include "Uav/UavModel.h"
#include "Tabs/DbBla/Defines.h"
#include "Tabs/DbBla/DbUavManager.h"

#define SECOND_TIMEOUT 1000


UavHistory::UavHistory(QSqlDatabase database, QObject *parent)
	: QObject(parent)
{
	m_timer.setInterval( SECOND_TIMEOUT );
	m_lifeTime = MAX_LIFE_TIME;
	m_isPaused = false;

	setDatabase( database );

	connect( &m_timer, SIGNAL(timeout()), SLOT(updateHistoryState()) );
	connect( this, SIGNAL(started(QDateTime,QDateTime, int)), SLOT(startInternal(QDateTime,QDateTime, int)) );
	connect( this, SIGNAL(paused()), SLOT(pauseInternal()) );
	connect( this, SIGNAL(resumed()), SLOT(resumeInternal()) );
	connect( this, SIGNAL(stopped()), SLOT(stopInternal()) );

	connect( this, SIGNAL(signalTimeoutMux(int)), this, SLOT(slotInitTimeoutMux(int)) );
}

UavHistory::~UavHistory()
{
	stop();
	m_database.close();
}

void UavHistory::setPathToSaveExport(const QString& path)
{
	m_pathToExport = path;
}

bool UavHistory::start(const QDateTime& startTime, const QDateTime& endTime, int mode)
{
	if( !m_database.isValid() ) {
		return false;
	}

	sendStatus( IUavHistoryListener::Loading );

	// perform cross thread synchronous call for any case
	if( this->thread() != QThread::currentThread() ) {
		QEventLoop loop;
		connect( this, SIGNAL(startFinished()), &loop, SLOT(quit()));
		emit started(startTime, endTime, mode);
		loop.exec();
	} else {
		emit started(startTime, endTime, mode);
	}

	sendStatus();

	return m_startResult;
}

void UavHistory::pause()
{
	emit paused();
}

bool UavHistory::xlsExport()
{
	QMap<QString, UavInfo> infoCollection;

	// get actual datetime and zero it msecs because we don't care about msecs
	QDateTime time = m_query.record().value( "datetime" ).toDateTime();
	time.setTime( time.time().addMSecs( - time.time().msec() ) );

	int ts = m_query.numRowsAffected();
	// collect UAV info for actual datetime
	QDateTime nextTime;

	int cnt = 0;

	do {
		UavInfo info;

		info.id = m_query.record().value( "id" ).toInt();
		info.uavId = m_query.record().value( "uavIdReal" ).toInt();

		info.lat = m_query.record().value( "latitude" ).toDouble();
		info.lon = m_query.record().value( "longitude" ).toDouble();
		info.alt = m_query.record().value( "altitude" ).toDouble();

		info.latStddev = m_query.record().value( "latitudeStdDev" ).toDouble();
		info.lonStddev = m_query.record().value( "longitudeStdDev" ).toDouble();

		info.speed = m_query.record().value( "speed" ).toDouble();
		info.yaw = m_query.record().value( "yaw" ).toDouble();
		info.restTime = m_query.record().value( "restTime" ).toTime();
		info.statusId = m_query.record().value( "statusTypeId" ).toInt();
		info.dateTime = m_query.record().value( "datetime" ).toDateTime();
		info.source = m_query.record().value( "sourceReal" ).toInt();
		info.historical = true;

		QString uavExtId = UavModel::getExtendedIdHist( info );
		infoCollection.insert( QString::number(cnt), info );

		// remember UAV role, because it is not keeped in UavInfo
		m_uavRoles.insert( info.name, m_query.record().value( "code" ).toString() );

		// remember last timestamp of UAV data changing
		m_uavLastDate.insert( info.name, info.dateTime );

		// get time zero msecs
		nextTime = m_query.record().value( "datetime" ).toDateTime();
		nextTime.setTime( nextTime.time().addMSecs( - nextTime.time().msec() ) );

		cnt++;
	} while( m_query.next() );

	return onSentToXl(infoCollection);
}

void UavHistory::resume()
{
	emit resumed();
}

void UavHistory::stop()
{
	if( getStatus() == IUavHistoryListener::Ready ) {
		return;
	}

	// perform cross thread synchronous call for any case
	if( this->thread() != QThread::currentThread() ) {
		QEventLoop loop;
		connect( this, SIGNAL(startFinished()), &loop, SLOT(quit()));
		emit stopped();
		loop.exec();
	} else {
		emit stopped();
	}
}

void UavHistory::setTimeoutMux(int timeout)
{
	emit signalTimeoutMux(timeout);
}

void UavHistory::setDatabase(const QSqlDatabase& database)
{
	stop();

	m_query.clear();

	if( m_database.isOpen() ) {
		m_database.close();
	}

	m_database = QSqlDatabase::cloneDatabase( database, "UAV history" );
	m_database.setConnectOptions( "MYSQL_OPT_RECONNECT=1" );

	if( !m_database.open() ) {
		sendStatus();
		return;
	}

	m_query = QSqlQuery( m_database );
	m_query.prepare( "SELECT info.*, uav.uavId as uavIdReal, sources.sourceId as sourceReal, uavRoles.code"
					 " FROM info"
					 " RIGHT JOIN uav on uav.id = info.uavID"
					 " LEFT JOIN uavRoles on uavRoles.id = uav.roleId"
					 " LEFT JOIN sources on sources.id = info.source"
					 " WHERE `datetime` >= :start AND `datetime` <= :end"
					 " GROUP BY uav.uavID, `datetime`, device, source"
					 " ORDER BY `datetime`" );

	sendStatus();
}

void UavHistory::setLifeTime(int msecs)
{
	m_lifeTime = msecs;
}

IUavHistoryListener::Status UavHistory::getStatus()
{
	if( !m_database.isOpen() ) {
		return IUavHistoryListener::NotReady;
	}

	if( m_timer.isActive() ) {
		return IUavHistoryListener::Playing;
	}

	if( m_isPaused ) {
		return IUavHistoryListener::Paused;
	}

	return IUavHistoryListener::Ready;
}

void UavHistory::moveToThread(QThread* thread)
{
	m_timer.moveToThread(thread);
	QObject::moveToThread(thread);
}

void UavHistory::startInternal(const QDateTime& start, const QDateTime& end, int mode)
{
	stop();

	m_query.bindValue( ":start", start );
	m_query.bindValue( ":end", end );
	m_query.exec();

	if( m_query.lastError().type() != QSqlError::NoError ) {
		log_error( m_query.lastError().databaseText() );
		m_startResult = false;
		emit startFinished();
		return;
	}

	if( !m_query.next() ) {
		m_startResult = false;

		emit startFinished();

		//stopInternal();

		return;
	}


	if(mode == 0) {
		updateHistoryState();

		m_timer.start();
		m_startResult = true;

		emit startFinished();
	} else if(mode == 1) {
		xlsExport();
		emit startFinished();
	}
	return;
}

void UavHistory::pauseInternal()
{
	if( m_isPaused ) return;

	m_timer.stop();
	m_isPaused = true;

	sendStatus();
}

void UavHistory::resumeInternal()
{
	if( !m_isPaused ) return;

	m_timer.start();
	m_isPaused = false;

	sendStatus();
}

void UavHistory::stopInternal()
{
	// send remove message for all UAVs
	foreach( QString id, m_knownUavsList.keys() ) {
		Uav uav = m_knownUavsList.value(id);
		//uav.name = id;
		//uav.historical = true;

		QString role = m_uavRoles.value( uav.name );
		if(role.isEmpty()) {
			role = ENEMY_UAV_ROLE;
		}

		foreach( IUavDbChangedListener* listener, m_receiversList ) {
			listener->onUavRemoved( uav, role );
		}
	}

	m_timer.stop();
	m_query.finish();
	m_knownUavsList.clear();
	m_uavRoles.clear();
	m_uavLastDate.clear();

	m_isPaused = false;

	sendStatus();

	emit stopFinished();
}

void UavHistory::updateHistoryState()
{
	QMap<QString, UavInfo> infoCollection;

	// get actual datetime and zero it msecs because we don't care about msecs
	QDateTime time = m_query.record().value( "datetime" ).toDateTime();
	time.setTime( time.time().addMSecs( - time.time().msec() ) );

	// collect UAV info for actual datetime
	QDateTime nextTime;
	do {
		UavInfo info;

		info.name = m_query.record().value( "uavName" ).toString();
        info.id = m_query.record().value( "uavIdReal" ).toInt();
		info.uavId = m_query.record().value( "uavIdReal" ).toInt() + 100000;

		info.lat = m_query.record().value( "latitude" ).toDouble();
		info.lon = m_query.record().value( "longitude" ).toDouble();
		info.alt = m_query.record().value( "altitude" ).toDouble();

		info.latStddev = m_query.record().value( "latitudeStdDev" ).toDouble();
		info.lonStddev = m_query.record().value( "longitudeStdDev" ).toDouble();

		info.speed = m_query.record().value( "speed" ).toDouble();
		info.yaw = m_query.record().value( "yaw" ).toDouble();
		info.restTime = m_query.record().value( "restTime" ).toTime();
		info.statusId = m_query.record().value( "statusTypeId" ).toInt();
		info.dateTime = m_query.record().value( "datetime" ).toDateTime();
		info.source = m_query.record().value( "sourceReal" ).toInt();
		info.historical = true;

		QString uavExtId = UavModel::getExtendedIdHist( info );
		infoCollection.insert( uavExtId, info );

		// remember UAV role, because it is not keeped in UavInfo
		m_uavRoles.insert( info.name, m_query.record().value( "code" ).toString() );

		// remember last timestamp of UAV data changing
		m_uavLastDate.insert( info.name, info.dateTime );

		if( !m_query.next() ) {
			stop();
			//break;
			return;
		}

		// get time zero msecs
		nextTime = m_query.record().value( "datetime" ).toDateTime();
		nextTime.setTime( nextTime.time().addMSecs( - nextTime.time().msec() ) );
	} while( nextTime == time );

	// send actual UAV info for all receivers
	foreach( UavInfo info, infoCollection ) {
		QString role = m_uavRoles.value( info.name );
		if(role.isEmpty()) {
			role = ENEMY_UAV_ROLE;
		}

		Uav uav;
		uav.uavId = info.uavId;
		uav.name =  UavModel::getExtendedIdHist( info );
		uav.historical = true;

		if (!m_knownUavsList.contains(uav.name)) {
			m_knownUavsList.insert(uav.name, uav);

			foreach (IUavDbChangedListener* receiver, m_receiversList){
				receiver->onUavAdded(uav, role);
			}
		}

		info.uavId = uav.uavId;
		foreach( IUavDbChangedListener* listener, m_receiversList ) {
			listener->onUavInfoChanged( info, role );
		}
	}

	// send remove message about outdated UAVs
	foreach( QString id, m_uavLastDate.keys() ) {
		if( m_uavLastDate.value( id ).msecsTo( time ) < m_lifeTime ) continue;

		Uav uav;
		//uav.uavId = id;
		uav.historical = true;

		QString role = m_uavRoles.value( uav.name );

		foreach( IUavDbChangedListener* listener, m_receiversList ) {
			listener->onUavRemoved( uav, role );
		}

		m_knownUavsList.remove(uav.name);
		m_uavRoles.remove( uav.name );
		m_uavLastDate.remove( uav.name );
	}
}

void UavHistory::slotInitTimeoutMux(int mux)
{
	m_timer.setInterval( SECOND_TIMEOUT / mux );
}

bool UavHistory::onSentToXl(QMap<QString, UavInfo>& infoCollection)
{
	//QString fileName = QFileDialog::getSaveFileName(0, "Save xls", "");
	// Create a database instance, set the connection string
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","excelexport");
	QString dsn = QString("DRIVER={Microsoft Excel Driver (*.xls)}; \
						  DSN=''; FIRSTROWHASNAMES=1; READONLY=FALSE; \
	CREATE_DB=\"%1\";DBQ=%2").arg(m_pathToExport).arg(m_pathToExport);
	db.setDatabaseName(dsn);
	QSqlError err;
	// Open database
	if (!db.open())
	{
		qDebug()<< "open false";
		err = db.lastError();
		//QMessageBox::about(NULL, "r", "open false");
	}
	// Create table
	QString sql = "create table sheet (id INTEGER, uavIdReal INTEGER, uavName TEXT, latitude DOUBLE, longtitude DOUBLE, altitude DOUBLE, \
			latitudeStdDev DOUBLE, longitudeStdDev DOUBLE, speed DOUBLE, yaw DOUBLE, restTime TEXT, \
			statusTypeId INTEGER, dateNtime TEXT, sourceReal INTEGER)";
			QSqlQuery query(db);
	if (!query.exec(sql))
	{
		qDebug()<< "create table false！";
		err = query.lastError();
		//QMessageBox::about(NULL, "r", "create table false！");
	}

	foreach (UavInfo info, infoCollection) {
		QSqlQuery query(db);
		query.prepare("INSERT INTO sheet(id, uavIdReal, uavName, latitude, longtitude, altitude, latitudeStdDev, longitudeStdDev, speed, yaw, restTime, statusTypeId, dateNtime, sourceReal) "
					  "VALUES		(:id, :uavIdReal, :latitude, :longtitude, :altitude, :latitudeStdDev, :longitudeStdDev, :speed, :yaw, :restTime, :statusTypeId, :dateNtime, :sourceReal)");
		query.bindValue(":id", info.id);
		query.bindValue(":uavName", info.name);
		query.bindValue(":uavIdReal", info.uavId);
		query.bindValue(":latitude", info.lat);
		query.bindValue(":longtitude", info.lon);
		query.bindValue(":altitude", info.alt);
		query.bindValue(":latitudeStdDev", info.latStddev);
		query.bindValue(":longitudeStdDev", info.lonStddev);
		query.bindValue(":speed", info.speed);
		query.bindValue(":yaw", info.yaw);
		query.bindValue(":restTime", info.restTime.toString());
		query.bindValue(":statusTypeId", info.statusId);
		query.bindValue(":dateNtime", info.dateTime.toString());
		query.bindValue(":sourceReal", info.source);

		query.exec();

		err = query.lastError();
		log_debug(err.text());
	}

	// Close database
	db.close();

	return true;
}


void UavHistory::sendStatus()
{
	sendStatus( getStatus() );
}

void UavHistory::sendStatus(IUavHistoryListener::Status status)
{
	foreach (IUavHistoryListener* receiver, m_receiversList){
		receiver->onStatusChanged( status );
	}
}
