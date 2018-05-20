#ifndef STYLEFACTORY_H
#define STYLEFACTORY_H
#include <QObject>
#include "PwGisClient.h"
#include <objects/IStyleFactory.h>

class IMapProvider;
class IIdGenerator;
class MapObjectStyle;

class GISCLIENT_EXPORT MapStyleFactory : public QObject, public IStyleFactory
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{2b99c98e-b4d5-441c-b5cb-5c6e8eb360f0}")
	Q_CLASSINFO("InterfaceID", "{a3ba29c7-f110-4e1d-af9f-ac7d52d3e681}")

private:
	IMapProvider* _mapProvider;
	IIdGenerator* _idGenerator;

public:
	MapStyleFactory(QObject* parent);
	MapStyleFactory(IMapProvider* mapProvider, IIdGenerator* idGenerator, QObject* parent);

public slots:
	virtual PwGisStyle* createNamedStyle(const QString& styleName);
	virtual PwGisStyle* createStyle();


	/**
	 * @brief Create Object Style.
	 * @return Created class.
	 */
	virtual MapObjectStyle* createObjectStyle();

};

#endif // STYLEFACTORY_H
