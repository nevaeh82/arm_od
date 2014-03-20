#ifndef NIIPPCONTROL_H
#define NIIPPCONTROL_H

#include <QWidget>
//#include <QDockWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QString>
#include <QComboBox>
#include <QLabel>
#include <QDateTime>
#include <QLineEdit>
#include <QStringList>
//#include <QLCDNumber>
#include "../MapInterface/MapController.h"

#include "INiippController.h"
#include "NIIPPControlWidget.h"

#include "../Interfaces/core/IRouter.h"
#include "../Interfaces/core/ITCPController.h"
#include "../Interfaces/ITCPClient.h"
#include "../Tabs/ITabManager.h"

#include "NIIPPParser.h"

#include "../Common/CommandMessage.h"

class NIIPPControlWidget;

class NIIPPControl : public QWidget, public INiiPPController
{
    Q_OBJECT
public:
    NIIPPControl(int id, QString name, QPointF latlon, IRouter *router, MapController* map_controller, ITabManager * parent_tab);
    ~NIIPPControl();

	void create();

	bool getState();

    void set_data(QByteArray data);

	int getAntenaType();
	double getRadiusCircle();
	double getRadiusSector();
	int getModeCurrentIndex();
	QWidget* getControlWidget();

private slots:
    void _slot_start_stop_clicked(bool state);

private:
    void _stop_commad();
    QByteArray _encode(QStringList list);

private:

	NIIPPControlWidget *_control_view;
    int             _id;
    int             _type;
    QString         _name;

    QPointF         _latlon;
    double          _width_angle;
    ITabManager*    _parent_tab;

    MapController*      _map_controller;

    double              _radius_sector;
    double              _radius_circle;
    double              _angel;

    IRouter*            _router;

//    QByteArray          _encode(QStringList list);

    QString             _number_command;

    int                 _antena_type;
    int                 _mode;
    QString             _command;

    int                 _mode_current_index;


    QPointF             _point_uvode_niipp;


signals:
    void signalChangeValuePower(int);
    void signalChangeAngle(double);

private slots:

    void _slot_change_angel(double value);
	void _slot_change_value_power(int value);
//    void _slot_change_antena(int value);
    void _slot_change_mode(int value);

    void _slot_enable_complex(bool state);

    void _slot_clear();

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
