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

	QMap<int, QDateTime> m_uavLastDate;
	QMap<int, QString> m_uavRoles;

	bool m_startResult;

	QMap<uint, Uav> m_knownUavsList;

public:
	explicit UavHistory(QSqlDatabase database, QObject *parent = 0);
	virtual ~UavHistory();

	bool start(const QDateTime& startTime, const QDateTime& endTime);
	void stop();

	void setDatabase(const QSqlDatabase& database);

	IUavHistoryListener::Status getStatus();

	void moveToThread(QThread *thread);

signals:
	void started(const QDateTime& start, const QDateTime& end);
	void startFinished();

protected slots:
	void startInternal(const QDateTime& start, const QDateTime& end);
	void updateHistoryState();

	void sendStatus();

};

#endif // UAVHISTORY_H
