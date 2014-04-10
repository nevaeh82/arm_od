#ifndef NIIPPCONTROL_H
#define NIIPPCONTROL_H

#include <QWidget>

#include "INiippController.h"
#include "NiippWidget.h"
#include "Niipp.h"

#include <Interfaces/IController.h>

class NiippController : public QObject, public IController<NiippWidget>, public INiiPPController
{
	Q_OBJECT
public:
	NiippController(int id, QString name, QPointF latlon, MapController* map_controller, ITabManager * parent_tab, QObject* parent = NULL);
	virtual ~NiippController();

	void appendView(NiippWidget* view);

	void create();

	bool getState();
	void setData(QByteArray data);

	int getAntenaType();
	double getRadiusCircle();
	double getRadiusSector();
	Niipp::WorkMode getModeCurrentIndex();
	QWidget* getControlWidget();

private:
	void stopCommad();
	QByteArray encode(QStringList list);

private:

	NiippWidget *m_view;
	Niipp  *m_controlModel;

signals:
	void angleChanged(double);

private slots:

	void changeAngel(double value);
	void changeValuePower(int value);
	void changeMode(int value);
	void enableComplex(bool state);
	void clear();
	void stopClicked();

public slots:
	virtual void setPower(double);
	virtual void setSwitchOn(bool state);
	virtual void setAntennaType(int value);
	virtual int getId();
	virtual void setPoint(QPointF coord);
	virtual void sendEnemyBpla(QPointF point, QPointF point_uvoda, double alt, double bearing);
	virtual void setAngle(double angle);
};

#endif // NIIPPCONTROL_H
