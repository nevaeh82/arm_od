#ifndef IUAVHISTORY_H
#define IUAVHISTORY_H

#include <QDateTime>

#include <BaseSubject.h>

#include "Interfaces/IDbChangedListener.h"

class IUavHistory : public BaseSubject<IDbChangedListener>
{
public:
	virtual ~IUavHistory() {}

	virtual bool dateExists(const QDateTime& date) = 0;

	virtual bool start(const QDateTime& start, const QDateTime& end) = 0;
	virtual void stop() = 0;
};

#endif // IUAVHISTORY_H
