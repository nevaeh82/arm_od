#include <QSqlError>
#include <QSqlRecord>

#include <Logger.h>

#include "UavHistory.h"
#include "Tabs/DbBla/Defines.h"

UavHistory::UavHistory(QSqlDatabase database, QObject *parent)
	: QObject(parent)
	, m_database(database)
{
	m_timer.setInterval( 1000 );

	connect( &m_timer, SIGNAL(timeout()), SLOT(updateHistoryState()) );

	m_query = QSqlQuery( m_database );
	m_query.prepare( "SELECT info.*, uav.uavId as uavIdReal, sources.sourceId as sourceReal, uavRoles.code"
					 " FROM info"
					 " RIGHT JOIN uav on uav.id = info.uavID"
					 " LEFT JOIN uavRoles on uavRoles.id = uav.roleId"
					 " LEFT JOIN sources on sources.id = info.source"
					 " WHERE `datetime` >= :start AND `datetime` <= :end"
					 " GROUP BY uav.uavID, `datetime`, device, altitude"
					 " ORDER BY `datetime`" );
}

UavHistory::~UavHistory()
{
	stop();
}

bool UavHistory::start(const QDateTime& start, const QDateTime& end)
{
	if( m_timer.isActive() ) {
		stop();
	}

	m_query.bindValue( ":start", start );
	m_query.bindValue( ":end", end );
	m_query.exec();

	if( m_query.lastError().type() != QSqlError::NoError ) {
		log_error( m_query.lastError().databaseText() );
		return false;
	}

	if( !m_query.next() ) {
		return false;
	}

	updateHistoryState();

	m_timer.start();
	return true;
}

void UavHistory::stop()
{
	/// \todo Call receivers method about history playback is stopped

	// send remove message for all UAVs
	foreach( int id, m_uavLastDate.keys() ) {
		Uav uav;
		uav.uavId = id;

		QString role = m_uavRoles.value( id );

		foreach( IUavDbChangedListener* listener, m_receiversList ) {
			listener->onUavRemoved( uav, role );
		}

	}

	m_timer.stop();
	m_query.finish();
	m_uavRoles.clear();
	m_uavLastDate.clear();
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

		foreach( IUavDbChangedListener* listener, m_receiversList ) {
			listener->onUavInfoChanged( info, role );
		}
	}

	// send remove message about outdated UAVs
	foreach( int id, m_uavLastDate.keys() ) {
		if( m_uavLastDate.value( id ).msecsTo( time ) < 600000 ) continue;

		Uav uav;
		uav.uavId = id;

		QString role = m_uavRoles.value( id );

		foreach( IUavDbChangedListener* listener, m_receiversList ) {
			listener->onUavRemoved( uav, role );
		}

		m_uavRoles.remove( id );
		m_uavLastDate.remove( id );
	}
}
