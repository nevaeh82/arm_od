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

public:
	explicit UavHistory(QSqlDatabase database, QObject *parent = 0);
	virtual ~UavHistory();

	bool start(const QDateTime& start, const QDateTime& end);
	void stop();

protected slots:
	void updateHistoryState();

};

#endif // UAVHISTORY_H
