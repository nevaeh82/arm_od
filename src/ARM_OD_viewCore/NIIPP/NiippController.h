#ifndef NIIPPCONTROL_H
#define NIIPPCONTROL_H

#include <QWidget>

#include "INiippController.h"
#include "NiippWidget.h"
#include "Niipp.h"

#include "NIIPPParser.h"
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
	virtual void set_power(double value);
	virtual void set_switch_on(bool state);
	virtual void set_antenna_type(int value);
	virtual int get_id();
	virtual void set_point(QPointF coord);
	virtual void send_evil(QPointF point, QPointF point_uvoda, double alt, double bearing);
	virtual void set_angle(double angle);
};

#endif // NIIPPCONTROL_H
