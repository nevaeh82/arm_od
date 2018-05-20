#ifndef TACTICOBJECTSFACTORY_H
#define TACTICOBJECTSFACTORY_H

#include <QObject>
#include <ITacticObjectsFactory.h>
#include <ITacticMap.h>


class IMapProvider;
class IIdGenerator;

class TacticObjectsFactory : public QObject, public ITacticObjectsFactory
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{f48aae17-88b7-4ad2-a2ea-c914e391ea7e}")
	Q_CLASSINFO("InterfaceID", "{57542671-a7a0-4b2d-8f6c-90019c96d09e}")
	Q_INTERFACES( ITacticObjectsFactory )

private:
	IMapProvider* _mapProvider;
	IIdGenerator* _idGenerator;
public:
	explicit TacticObjectsFactory(QObject *parent = 0);
	explicit TacticObjectsFactory(IMapProvider* map, IIdGenerator* idGenrator, QObject *parent );

signals:

public slots:
	virtual Post* createPost();
	virtual RadioSource* createRadioSource();
	virtual Bearing* createBearing();
};

#endif // TACTICOBJECTSFACTORY_H
