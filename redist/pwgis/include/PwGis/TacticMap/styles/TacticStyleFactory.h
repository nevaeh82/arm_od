#ifndef TACTICSTYLEFACTORY_H
#define TACTICSTYLEFACTORY_H
#include <QObject>
#include "ITacticStyleFactory.h"
#include "../TacticMap_global.h"

class IStyleFactory;

class TACTICMAP_EXPORT TacticStyleFactory: public QObject, public ITacticStyleFactory
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{d1e3a4c6-706d-493b-ab2d-7e1d9054fb2d}")
	Q_CLASSINFO("InterfaceID", "{64ca29d4-f1a6-4c55-a19a-3926b0cdc402}")

	Q_INTERFACES( ITacticStyleFactory )
private:
	IStyleFactory* _gisStyleFactory;

public:
	explicit TacticStyleFactory(IStyleFactory* gisStyleFactory, QObject* parent = NULL);

public slots:
	BearingStyle* createBearingStyle();
	PostStyle* createPostStyle();
	RadioSourceStyle* createRadioSourceStyle();
	MapObjectStyle* createMapObjectStyle();
};

#endif // TACTICSTYLEFACTORY_H
