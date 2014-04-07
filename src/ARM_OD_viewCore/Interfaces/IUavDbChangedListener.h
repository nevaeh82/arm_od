#ifndef IUAVDBCHANGEDLISTENER_H
#define IUAVDBCHANGEDLISTENER_H

struct Uav;
struct UavInfo;

class IUavDbChangedListener{

	public:
		virtual ~IUavDbChangedListener(){}

		virtual void onUavAdded(const Uav& uav) = 0;
		virtual void onUavRemoved(const Uav& uav) = 0;

		virtual void onUavInfoChanged(const UavInfo& uavInfo) = 0;
};

#endif // IUAVDBCHANGEDLISTENER_H
