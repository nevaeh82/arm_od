#ifndef MAPFORMATSCOLLECTION_H
#define MAPFORMATSCOLLECTION_H
#include <QObject>
#include <QList>
#include <QSet>    //required for QList
#include <QVector> //required for QList

#include "PwGisClient.h"
#include "MapFormatInfo.h"

class GISCLIENT_EXPORT MapFormatsCollection : public QObject, public QList<MapFormatInfo*>
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{BA66659A-9E99-482C-B569-86A7C916CAA3}")
	Q_CLASSINFO("InterfaceID", "{3FDCF156-DB8A-450B-8ABE-CDD2C6E0E068}")
public:
	MapFormatsCollection(QObject* parent = 0) :
		QObject(parent),
		QList<MapFormatInfo*>()
	{
	}

	MapFormatsCollection(const MapFormatsCollection& mfc) :
		QObject(mfc.parent()),
		QList<MapFormatInfo*>()
	{
		append(mfc);
	}
public slots:
	int getCount() const { return count(); }
	MapFormatInfo* get(int idx) { return at(idx); }
	void add(MapFormatInfo* mfi) { append(mfi); }
	void clearAll() { clear(); }

};


#endif // MAPFORMATSCOLLECTION_H
