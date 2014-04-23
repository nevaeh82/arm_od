#ifndef IUAVHISTORY_H
#define IUAVHISTORY_H

#include <QDateTime>
#include <QSqlDatabase>

#include <BaseSubject.h>

#include "Interfaces/IUavHistoryListener.h"

class IUavHistory : public BaseSubject<IUavHistoryListener>
{
public:
	virtual ~IUavHistory() {}

	virtual bool start(const QDateTime& start, const QDateTime& end) = 0;
	virtual void stop() = 0;

	virtual void setDatabase(const QSqlDatabase& database) = 0;
	virtual void setLifeTime(int msecs) = 0;

	virtual IUavHistoryListener::Status getStatus() = 0;
};

#endif // IUAVHISTORY_H
