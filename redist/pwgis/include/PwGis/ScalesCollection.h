#ifndef SCALESCOLLECTION_H
#define SCALESCOLLECTION_H
#include <QObject>
#include <QList>
#include <QSet>    //required for QList
#include <QVector> //required for QList

#include "PwGisClient.h"

class GISCLIENT_EXPORT ScalesCollection : public QObject, public QList<double>
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{8c3d35cd-316b-4142-9997-c28f6cc916f6}")
	Q_CLASSINFO("InterfaceID", "{9f6300de-75ea-43df-9a05-620fb2f99f81}")

public:
	ScalesCollection(QObject* parent = 0) :
		QObject(parent),
		QList<double>()
	{
	}

	ScalesCollection(const ScalesCollection& f) :
		QObject(f.parent()),
		QList<double>()
	{
		append(f);
	}

public slots:
	int getCount() const { return count(); }
	double get(int ind) { return at(ind); }
	void add(double d) { append(d); }
	void clearAll() { clear(); }

};

inline uint qHash(const double& d)
{
	return uint(d);
}

#endif // SCALESCOLLECTION_H
