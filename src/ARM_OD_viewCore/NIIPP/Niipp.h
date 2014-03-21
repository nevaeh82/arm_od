#ifndef NIIPPCONTROLMODEL_H
#define NIIPPCONTROLMODEL_H

#include "../MapInterface/MapController.h"
#include "../Interfaces/core/IRouter.h"
#include "../Tabs/ITabManager.h"

#include "../Common/CommandMessage.h"

class Niipp
{
public:
	Niipp(int id, QString name, QPointF latlon, IRouter *router, MapController* mapController, ITabManager* parentTab);
	~Niipp();

	void create();


	void setAntennaType(int value);
	void setModeCurrentIndex(int value);
	void setAntenaIndex(int value);
	void setSBpowerValue(int value);
	void setAngel(double value);
	void setPoint(QPointF coord);

	void stopCommad();
	void changeValuePower(int value);
	void enableComplex(bool state);
	void clear();
	void sendEvil(QPointF point, QPointF point_uvoda, double alt, double bearing);
	QByteArray encode(QStringList list);
	void changeMode(int value);

	int getId();
	int getAntenaType();
	double getRadiusCircle();
	double getRadiusSector();
	int getModeCurrentIndex();

private:
	int             m_id;
	int             m_type;
	double          m_widthAngle;
	double          m_radiusSector;
	double          m_radiusCircle;
	double          m_angel;
	int             m_modeCurrentIndex;
	int             m_antenaType;
	int             m_mode;

	int             m_antenaIndex;
	int             m_sbPowerValue;

	QString         m_name;
	QString         m_numberCommand;
	QString         m_command;
	QPointF         m_latlon;
	QPointF         m_pointUvodeNiipp;

	ITabManager*    m_parentTab;
	MapController*  m_mapController;
	IRouter*        m_router;
};

#endif // NIIPPCONTROLMODEL_H
