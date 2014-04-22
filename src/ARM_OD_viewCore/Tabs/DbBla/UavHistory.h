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

public:
	explicit UavHistory(QSqlDatabase database, QObject *parent = 0);
	virtual ~UavHistory();

	bool start(const QDateTime& startTime, const QDateTime& endTime);
	void stop();

signals:
	void started(const QDateTime& start, const QDateTime& end);
	void startFinished();

protected slots:
	void startInternal(const QDateTime& start, const QDateTime& end);
	void updateHistoryState();

};

#endif // UAVHISTORY_H
