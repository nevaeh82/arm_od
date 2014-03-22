#ifndef ITCPAISZONEMANAGER_H
#define ITCPAISZONEMANAGER_H

class QString;

class ITcpAISZoneManager
{

public:
	virtual ~ITcpAISZoneManager(){}

	virtual QString getZone(const double& latitude1, const double& longitude1,
							const double& latitude2, const double& longitude2) = 0;
};

#endif // ITCPAISZONEMANAGER_H
