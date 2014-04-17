#ifndef INIIPPCONTROLLER_H
#define INIIPPCONTROLLER_H

#include <QPointF>

class INiippController
{
public:
	virtual ~INiippController() {}

	virtual void setPower(double value) = 0;
	virtual void setSwitchOn(bool state) = 0;
	virtual void setAntennaType(int value) = 0;
	virtual void sendEnemyBpla(QPointF point, QPointF point_uvoda, double alt, double bearing) = 0;
	virtual int  getId()    = 0;

	virtual void setAngle(double angle)    = 0;

	virtual void setPoint(QPointF coord) = 0;
};

#endif // INIIPPCONTROLLER_H
