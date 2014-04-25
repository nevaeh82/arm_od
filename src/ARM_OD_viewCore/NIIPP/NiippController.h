#ifndef NIIPPCONTROL_H
#define NIIPPCONTROL_H

#include <QWidget>

#include <Interfaces/IController.h>

#include <QProcess>

#include "Interfaces/IUavDbChangedListener.h"
#include "Interfaces/IRpcListener.h"

#include "Map/IMapController.h"

#include "RPC/RpcDefines.h"

#include "INiippController.h"
#include "NiippWidget.h"
#include "Niipp.h"

class NiippController : public QObject, public IController<NiippWidget>, public INiippController,
        public IUavDbChangedListener, public IRpcListener
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

    virtual void onMethodCalled(const QString& method, const QVariant& argument);


private:
	void stopCommad();
	QByteArray encode(QStringList list);
    int openRDP(int id);

private slots:
	void changeAngel(double value);
	void changeValuePower(int value);
	void changeMode(int value);
	void enableComplex(bool state);
	void clear();
    void slotStopClicked();
    void slotStartClicked();
    void slotOpenRDP();

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
