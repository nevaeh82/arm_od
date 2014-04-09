#ifndef ITCPAISZONEMANAGER_H
#define ITCPAISZONEMANAGER_H

class QString;
class AISZone;

class ITcpAISZoneManager
{

public:
	virtual ~ITcpAISZoneManager(){}

	virtual QString getZone(const double& latitude1, const double& longitude1,
							const double& latitude2, const double& longitude2) = 0;
	virtual QString getZone(const AISZone& aisZone) = 0;
	virtual AISZone getLastZone() = 0;
};

#endif // ITCPAISZONEMANAGER_H
