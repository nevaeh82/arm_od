#ifndef IDGENERATOR_H
#define IDGENERATOR_H
#include <QObject>
#include "PwGisClient.h"
#include "IIdGenerator.h"

class GISCLIENT_EXPORT IdGenerator: public QObject, public IIdGenerator
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{0a2889f7-77b7-40db-a3cb-a96491f0961d}")
	Q_CLASSINFO("InterfaceID", "{8745fa49-63f6-4f7c-9259-db033f2cf3c6}")

	qlonglong _idCounter;

public:
	IdGenerator(QObject* parent = NULL);

public slots:
	virtual QString GetNewID();
};

#endif // IDGENERATOR_H
