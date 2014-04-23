#ifndef NIIPPCONTROL_H
#define NIIPPCONTROL_H

#include <QWidget>

#include <Interfaces/IController.h>

#include "Interfaces/IUavDbChangedListener.h"

#include "Map/IMapController.h"

#include "INiippController.h"
#include "NiippWidget.h"
#include "Niipp.h"

class NiippController : public QObject, public IController<NiippWidget>, public INiippController,
		public IUavDbChangedListener
{
	Q_OBJECT

private:
	NiippWidget* m_view;
	Niipp* m_model;
	IMapController* m_mapController;

public:
	NiippController(int id, QString name, QPointF latlon, IMapController* mapController,
					ITabManager * parentTab, QObject* parent = NULL);
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

	// interface IUavDbChangedListener
	virtual void onUavAdded(const Uav&, const QString&) {}
	virtual void onUavRemoved(const Uav&, const QString&) {}
	virtual void onUavInfoChanged(const UavInfo& uavInfo, const QString& uavRole);

private:
	void stopCommad();
	QByteArray encode(QStringList list);

private slots:
	void changeAngel(double value);
	void changeValuePower(int value);
	void changeMode(int value);
	void enableComplex(bool state);
	void clear();
    void slotStopClicked();
    void slotStartClicked();

public slots:
	virtual void setPower(double value);
	virtual void setSwitchOn(bool state);
	virtual void setAntennaType(int value);
	virtual int getId();
	virtual void setPoint(QPointF coord);
	virtual void sendEnemyBpla(QPointF point, QPointF point_uvoda, double alt, double bearing);
	virtual void setAngle(double angle);

signals:
	void angleChanged(double);
};

#endif // NIIPPCONTROL_H
