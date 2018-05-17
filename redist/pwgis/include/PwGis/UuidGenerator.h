#ifndef UUIDGENERATOR_H
#define UUIDGENERATOR_H
#include <QObject>
#include <QUuid>
#include "PwGisClient.h"
#include "IIdGenerator.h"

class GISCLIENT_EXPORT UuidGenerator: public QObject, public IIdGenerator
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{0f396890-a43e-40f2-89b1-86ac99321be5}")
	Q_CLASSINFO("InterfaceID", "{af9cd8a8-68e3-47d4-9dd5-11ea17d10f31}")

public:
	UuidGenerator(QObject* parent = NULL);

public slots:
	virtual QString GetNewID();
};

#endif // UUIDGENERATOR_H
