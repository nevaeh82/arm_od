#include <QSqlError>
#include <QSqlRecord>
#include <QEventLoop>

#include <Logger.h>

#include "UavHistory.h"
#include "Tabs/DbBla/Defines.h"

UavHistory::UavHistory(QSqlDatabase database, QObject *parent)
	: QObject(parent)
{
	m_timer.setInterval( 1000 );

	setDatabase( database );

	connect( &m_timer, SIGNAL(timeout()), SLOT(updateHistoryState()) );
	connect( this, SIGNAL(started(QDateTime,QDateTime)), SLOT(startInternal(QDateTime,QDateTime)) );
}

UavHistory::~UavHistory()
{
	stop();
	m_database.close();
}

bool UavHistory::start(const QDateTime& startTime, const QDateTime& endTime)
{
	if( !m_database.isValid() ) {
		return false;
	}

	emit started(startTime, endTime);

	QEventLoop loop;
	connect( this, SIGNAL(startFinished()), &loop, SLOT(quit()));
	loop.exec();

	sendStatus();

	return m_startResult;
}

void UavHistory::stop()
{
	/// \todo Call receivers method about history playback is stopped

	// send remove message for all UAVs
	foreach( int id, m_knownUavsList.keys() ) {
		Uav uav;
		uav.uavId = id;
		uav.historical = true;

		QString role = m_uavRoles.value( id );

		foreach( IUavDbChangedListener* listener, m_receiversList ) {
			listener->onUavRemoved( uav, role );
		}
	}

	m_timer.stop();
	m_query.finish();
	m_knownUavsList.clear();
	m_uavRoles.clear();
	m_uavLastDate.clear();

	sendStatus();
}

void UavHistory::setDatabase(const QSqlDatabase& database)
{
	stop();

	m_query.clear();

	if( m_database.isOpen() ) {
		m_database.close();
	}

	m_database = QSqlDatabase::cloneDatabase( database, "UAV history" );

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
					 " GROUP BY uav.uavID, `datetime`, device, altitude"
					 " ORDER BY `datetime`" );

	sendStatus();
}

IUavHistoryListener::Status UavHistory::getStatus()
{
	if( !m_database.isOpen() ) {
		return IUavHistoryListener::NotReady;
	}

	if( m_timer.isActive() ) {
		return IUavHistoryListener::Playing;
	}

	return IUavHistoryListener::Ready;
}

void UavHistory::moveToThread(QThread* thread)
{
	m_timer.moveToThread(thread);
	QObject::moveToThread(thread);
}

void UavHistory::startInternal(const QDateTime& start, const QDateTime& end)
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
		return;
	}

	updateHistoryState();

	m_timer.start();
	m_startResult = true;

	emit startFinished();
	return;
}

void UavHistory::updateHistoryState()
{
	QMap<int, UavInfo> infoCollection;

	// get actual datetime and zero it msecs because we don't care about msecs
	QDateTime time = m_query.record().value( "datetime" ).toDateTime();
	time.setTime( time.time().addMSecs( - time.time().msec() ) );

	// collect UAV info for actual datetime
	QDateTime nextTime;
	do {
		UavInfo info;

		info.id = m_query.record().value( "id" ).toInt();
		info.uavId = m_query.record().value( "uavIdReal" ).toInt();
		info.lat = m_query.record().value( "latitude" ).toDouble();
		info.lon = m_query.record().value( "longitude" ).toDouble();
		info.alt = m_query.record().value( "altitude" ).toDouble();
		info.speed = m_query.record().value( "speed" ).toDouble();
		info.yaw = m_query.record().value( "yaw" ).toDouble();
		info.restTime = m_query.record().value( "restTime" ).toTime();
		info.statusId = m_query.record().value( "statusTypeId" ).toInt();
		info.dateTime = m_query.record().value( "datetime" ).toDateTime();
		info.source = m_query.record().value( "sourceReal" ).toInt();
		info.historical = true;

		m_uavRoles.insert( info.uavId, m_query.record().value( "code" ).toString() );
		infoCollection.insert( info.uavId, info );

		// remember last timestamp of UAV data changing
		m_uavLastDate.insert( info.uavId, info.dateTime );

		if( !m_query.next() ) {
			stop();
			break;
		}

		// get time zero msecs
		nextTime = m_query.record().value( "datetime" ).toDateTime();
		nextTime.setTime( nextTime.time().addMSecs( - nextTime.time().msec() ) );
	} while( nextTime == time );

	// send actual UAV info for all receivers
	foreach( UavInfo info, infoCollection ) {
		QString role = m_uavRoles.value( info.uavId );

		Uav uav;
		uav.uavId = info.uavId;
		uav.name = QObject::tr( "%1-H" ).arg( info.uavId );
		uav.historical = true;

		if (!m_knownUavsList.contains(uav.uavId)) {
			m_knownUavsList.insert(uav.uavId, uav);

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
	foreach( int id, m_uavLastDate.keys() ) {
		if( m_uavLastDate.value( id ).msecsTo( time ) < 600000 ) continue;

		Uav uav;
		uav.uavId = id;
		uav.historical = true;

		QString role = m_uavRoles.value( id );

		foreach( IUavDbChangedListener* listener, m_receiversList ) {
			listener->onUavRemoved( uav, role );
		}

		m_knownUavsList.remove(id);
		m_uavRoles.remove( id );
		m_uavLastDate.remove( id );
	}
}

void UavHistory::sendStatus()
{
	foreach (IUavHistoryListener* receiver, m_receiversList){
		receiver->onStatusChanged( getStatus() );
	}
}
