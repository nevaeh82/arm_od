#ifndef IUAVHISTORY_H
#define IUAVHISTORY_H

#include <QDateTime>

#include <BaseSubject.h>

#include "Interfaces/IDbChangedListener.h"

class IUavHistory : public BaseSubject<IDbChangedListener>
{
public:
	virtual ~IUavHistory() {}

	virtual bool dateExists(const QDateTime& date);

	virtual bool start(const QDateTime& start, const QDateTime& end);
	virtual void stop();
};

#endif // IUAVHISTORY_H
