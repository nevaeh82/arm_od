#ifndef NIIPPCONTROLMODEL_H
#define NIIPPCONTROLMODEL_H

#include <QPointF>

#include "Tabs/ITabManager.h"
#include "Common/CommandMessage.h"

class Niipp
{
//Declare mode enum
public:
	enum WorkMode {
		disabled = -1,
		radiation,
		adduction,
		roundView
	};

public:
	Niipp(int id, QString name, QPointF latlon, ITabManager* parentTab);
	~Niipp() {}

	void setAntennaType(int value);
	void setMode(int value);
	void setAntenaIndex(int value);
	void setSBpowerValue(int value);
	void setAngel(double value);
	void setPoint(const QPointF& coord);

	void stopCommad();
	void startCommand();
	void changeValuePower(int value);
	void enableComplex(bool state);
	void clear();
	void sendEvil(const QPointF& point, const QPointF& point_uvoda, double alt, double bearing);
	QByteArray encode(QStringList list);
	void changeMode(int value);

	double getRadius() const;

	inline int getId() const { return m_id; }
	inline int getAntenaType() const { return m_antenaType; }
	inline double getRadiusCircle() const { return m_radiusCircle; }
	inline double getRadiusSector() const { return m_radiusSector; }
	inline Niipp::WorkMode getModeCurrentIndex() const { return m_modeCurrentIndex; }
	inline QPointF getPosition() const { return m_latlon; }
	inline QPointF getPoint() const { return m_pointUvodeNiipp; }
	inline QString getName() const { return m_name; }
	inline double  getAngle() const { return m_angel; }

private:
	int             m_id;
	int             m_type;
	double          m_widthAngle;
	double          m_radiusSector;
	double          m_radiusCircle;
	double          m_angel;
	WorkMode        m_modeCurrentIndex;
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

	bool            m_enable;
};

#endif // NIIPPCONTROLMODEL_H
