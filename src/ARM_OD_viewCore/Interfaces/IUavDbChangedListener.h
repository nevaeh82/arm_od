#ifndef IUAVDBCHANGEDLISTENER_H
#define IUAVDBCHANGEDLISTENER_H

#include <QVector>
#include <QPointF>

class QString;

struct Uav;
struct UavInfo;

class IUavDbChangedListener{

	public:
		virtual ~IUavDbChangedListener(){}

		virtual void onUavAdded(const Uav& uav, const QString& uavRole) = 0;
		virtual void onUavRemoved(const Uav& uav, const QString& uavRole) = 0;

		virtual void onUavInfoChanged(const UavInfo& uavInfo, const QString& uavRole,
									  const QVector<QPointF> &tail = QVector<QPointF>(),
									  const QVector<QPointF> &tailStdDev = QVector<QPointF>()) = 0;
};

#endif // IUAVDBCHANGEDLISTENER_H
