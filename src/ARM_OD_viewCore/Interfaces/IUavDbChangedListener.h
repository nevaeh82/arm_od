#ifndef IUAVDBCHANGEDLISTENER_H
#define IUAVDBCHANGEDLISTENER_H

class QString;

struct Uav;
struct UavInfo;

class IUavDbChangedListener{

	public:
		virtual ~IUavDbChangedListener(){}

		virtual void onUavAdded(const Uav& uav, const QString& uavRole) = 0;
		virtual void onUavRemoved(const Uav& uav, const QString& uavRole) = 0;

		virtual void onUavInfoChanged(const UavInfo& uavInfo, const QString& uavRole) = 0;
};

#endif // IUAVDBCHANGEDLISTENER_H
