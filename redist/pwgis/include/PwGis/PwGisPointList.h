#ifndef PWGISPOINTLIST_H
#define PWGISPOINTLIST_H
#include <QList>
#include <QSet>
#include <QVector>

#include <QObject>
#include <QMutex>

#include "PwGisClient.h"
#include "PwGisLonLat.h"

class GISCLIENT_EXPORT PwGisPointList : public QObject, protected QList<PwGisLonLat*>
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{F5ABECDE-ADF9-4897-87AA-18C13A99D021}")
	Q_CLASSINFO("InterfaceID", "{8D3CD71E-115D-4D59-ABE7-30461631BD63}")

public:
	PwGisPointList(QObject* parent = 0) :
		QObject(parent), _mutex(QMutex::Recursive)
	{
	}

	PwGisPointList(const PwGisPointList& another):
		QObject(),
		QList<PwGisLonLat*>(another) ,
		_mutex(QMutex::Recursive)
	{
	}

	virtual ~PwGisPointList();
	QMutex _mutex;

	using QList<PwGisLonLat*>::const_iterator;
	using QList<PwGisLonLat*>::begin;
	using QList<PwGisLonLat*>::end;
	using QList<PwGisLonLat*>::at;
	using QList<PwGisLonLat*>::count;
	using QList<PwGisLonLat*>::length;

public slots:

	int getCount() const { return count(); }

	PwGisLonLat* getLonLat(int idx)
	{
		QMutexLocker locker(&_mutex);
		return at(idx);
	}
	PwGisLonLat* last()
	{
		QMutexLocker locker(&_mutex);
		return QList<PwGisLonLat*>::last();
	}
	PwGisLonLat* first()
	{
		QMutexLocker locker(&_mutex);
		return QList<PwGisLonLat*>::first();
	}
	void append(PwGisLonLat* point)
	{
		QMutexLocker locker(&_mutex);
		point->setParent(this);
		QList<PwGisLonLat*>::append(point);
	}
	void addLonLat(PwGisLonLat* point)
	{
		append(point);
	}

	void removeLonLat(int idx)
	{
		QMutexLocker locker(&_mutex);
		removeAt(idx);
	}
	void removeLast()
	{
		QMutexLocker locker(&_mutex);
		QList<PwGisLonLat*>::removeLast();
	}
	void removeFirst()
	{
		QMutexLocker locker(&_mutex);
		QList<PwGisLonLat*>::removeFirst();
	}

	void clear();
};

#endif // PWGISPOINTLIST_H
