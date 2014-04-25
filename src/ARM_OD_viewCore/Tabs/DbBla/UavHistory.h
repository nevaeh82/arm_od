#ifndef UAVHISTORY_H
#define UAVHISTORY_H

#include <QMap>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "Interfaces/IUavHistory.h"

class UavHistory : public QObject, public IUavHistory
{
	Q_OBJECT

private:
	QTimer m_timer;

	QSqlDatabase m_database;
	QSqlQuery m_query;

	QMap<uint, QDateTime> m_uavLastDate;
	QMap<uint, QString> m_uavRoles;
	QMap<uint, Uav> m_knownUavsList;

	bool m_startResult;
	bool m_isPaused;
	int m_lifeTime;

public:
	explicit UavHistory(QSqlDatabase database, QObject *parent = 0);
	virtual ~UavHistory();

	bool start(const QDateTime& startTime, const QDateTime& endTime);
	void pause();
	void resume();
	void stop();

	void setDatabase(const QSqlDatabase& database);
	void setLifeTime(int msecs);

	IUavHistoryListener::Status getStatus();

	void moveToThread(QThread *thread);

signals:
	void started(const QDateTime& start, const QDateTime& end);
	void stopped();
	void paused();
	void resumed();

	void startFinished();
	void stopFinished();

protected slots:
	void startInternal(const QDateTime& start, const QDateTime& end);
	void stopInternal();
	void pauseInternal();
	void resumeInternal();

	void updateHistoryState();

	void sendStatus();
	void sendStatus(IUavHistoryListener::Status status);

};

#endif // UAVHISTORY_H
