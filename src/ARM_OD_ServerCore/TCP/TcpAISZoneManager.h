#ifndef TCPAISZONEMANAGER_H
#define TCPAISZONEMANAGER_H

#include <QObject>

#include "./Interfaces/ITcpAISZoneManager.h"
#include "TcpAISZoneDefines.h"
#include "TcpDefines.h"

class TcpAISZoneManager : public QObject, public ITcpAISZoneManager
{
	Q_OBJECT

private:
	AISZone m_lastZone;

public:
	explicit TcpAISZoneManager(QObject* parent = NULL);
	virtual ~TcpAISZoneManager();

	// ITcpAISZoneManager interface
	virtual QString getZone(const double& latitude1, const double& longitude1,
							const double& latitude2, const double& longitude2);
	virtual QString getZone(const AISZone& aisZone);
	virtual AISZone getLastZone();
};

#endif // TCPAISZONEMANAGER_H
