#include "NIIPPControl.h"

#include <QDebug>

const double _zone[24] = {1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5,
                       5, 6, 7, 8, 9, 10, 11, 12, 14, 16,
                       18, 20, 22, 24, 28, 30};
const double _zone_dir[28] = {2.5, 3, 4, 5,
                       6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20,
                       22, 26, 29, 33, 37, 41, 47, 52, 57, 62, 68, 72, 76};

NIIPPControl::NIIPPControl(int id, QString name, QPointF latlon, IRouter *router, MapController* map_controller, ITabManager* parent_tab):
	_number_command("0200"),
	_antena_type(0),
	_width_angle(25),
	_mode_current_index(-1)
{

	_control_view = new NIIPPControlWidget(this, 0);

    _id = id;
    _name = name;
    _latlon = latlon;
    _parent_tab = parent_tab;
    _router = router;
    _radius_sector = 0;
    _radius_circle = 0;
    _angel = 0;
    _map_controller = map_controller;


    _map_controller->get_map_client(1)->set_niipp_controller(this);

//    this->setStyleSheet("background-color: red; border-width: 10px; border-color: black");
    create();


//    connect(_sl_angel, SIGNAL(valueChanged(int)), this, SLOT(_slot_change_angel(int)));

//    NIIPPParser *parser = new NIIPPParser();
//    parser->set_router(_router);

//    _router->get_tcp_controller()->add_connection(_ip, _port, parser, 0);
}

NIIPPControl::~NIIPPControl()
{
}

void NIIPPControl::create()
{
//    _map_controller->get_map_client(1)->slot_create_niipp_sector(0, "NIIPP1", 30.531368, 60.074592, 45);
    //    _map_controller->get_map_client(1)->slot_update_sector(0, 1000, 45);

    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);

    ds << _name;
    ds << _latlon;
    ds << _width_angle;

    _map_controller->get_map_client(1)->slot_update_sector(_id, 0, _angel, ba);

    connect(this, SIGNAL(signalChangeAngle(double)), this, SLOT(_slot_change_angel(double)));
}

bool NIIPPControl::getState()
{
	return _control_view->getEnableComplexState();
}

void NIIPPControl::set_data(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);

    int mode;

    ds >> mode;

	_control_view->setLeStatusText(mode);

}

void NIIPPControl::_slot_start_stop_clicked(bool state)
{

	if(!state)
	{
		_stop_commad();
	}
}

void NIIPPControl::_stop_commad()
{    
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);

    QDateTime dt = QDateTime::currentDateTime();
    QDate date = dt.date();
    ds << _id;
    ds << date;
    QTime time = dt.time();
    ds << time;

    QString status = "0300";
    ds << status;

    QPointF point1;
    point1.setX(0);
    point1.setY(0);
    ds << point1;
    QString s1 = "N";
    ds << s1;
    s1 = "E";
    ds << s1;
    int alt = 0;
    ds << alt;
    int velocity_dir = 0;
    ds << velocity_dir;
    int zone = 0;
    ds << zone;
    QPointF point2;
    point2.setX(0);
    point2.setY(0);
    ds << point2;
    s1 = "N";
    ds << s1;

    s1 = "E";
    ds << s1;

    CommandMessage* msg = new CommandMessage(COMMAND_SET_NIIPP_BPLA, ba);
    _parent_tab->send_data(0, msg);
}

void NIIPPControl::_slot_enable_complex(bool state)
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);

    QString status;

    if(state)
    {
        status = "0000";
		//_pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);"));
    }
    else
    {
        status = "0001";
		//_pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
    }

//    QStringList list;
    QDateTime dt = QDateTime::currentDateTime();
    QDate date = dt.date();
    ds << _id;
    ds << date;
//    list.append(date.toString("ddMMyy"));
    QTime time = dt.time();
    ds << time;
//    list.append(time.toString("HHmmss"));

//    list.append(status);
    ds << status;

//    list.append("000000");
//    QString s1 = "000000";
//    ds << s1;
    QPointF point1;
    point1.setX(0);
    point1.setY(0);
    ds << point1;
    QString s1 = "N";
    ds << s1;
//    list.append("N");

//    list.append("000000");
//    s1 = "000000";
//    ds << s1;
//    list.append("E");
    s1 = "E";
    ds << s1;


//    list.append("0");
    int alt = 0;
    ds << alt;
//    list.append("0");
    int velocity_dir = 0;
    ds << velocity_dir;
//    list.append("01");
    int zone = 0;
    ds << zone;

//    list.append("000000");
//    s1 = "000000";
//    ds << s1;
    QPointF point2;
    point2.setX(0);
    point2.setY(0);
    ds << point2;
//    list.append("N");
    s1 = "N";
    ds << s1;

//    list.append("000000");
//    s1 = "000000";
//    ds << s1;
//    list.append("W");
    s1 = "E";
    ds << s1;


//    QByteArray ba = _encode(list);

    CommandMessage* msg = new CommandMessage(COMMAND_SET_NIIPP_BPLA, ba);
    _parent_tab->send_data(0, msg);

//

    //    _router->get_tcp_controller()->get_client(_ip, _port, 0)->set(ba);

}

void NIIPPControl::_slot_clear()
{
    _point_uvode_niipp.setX(0);
    _point_uvode_niipp.setY(0);

    _map_controller->get_map_client(1)->slot_remove_point_uvoda();
}

void NIIPPControl::_slot_change_value_power(int value)
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);

    ds << _name;
    ds << _latlon;
    ds << _width_angle;


    if(_antena_type == 0)
    {
        _map_controller->get_map_client(1)->slot_update_sector(_id, _zone_dir[value], _angel, ba);
        _radius_sector = _zone_dir[value];
//        _le_distance->setText(QString("%1 км").arg(QString::number(_radius_sector)));
//        if(_pb_start->isChecked())
//        {
//            _le_status->setText(tr("Облучение"));
//        }
    }
    if(_antena_type == 1)
    {
        _map_controller->get_map_client(1)->slot_niipp_power_cicle(_id, _zone[value], ba);
        _radius_circle = _zone[value];
//        _le_distance->setText(QString("%1 км").arg(QString::number(_radius_circle)));
//        if(_pb_start->isChecked())
//        {
//            _le_status->setText(tr("Облучение"));
//        }
    }

//    emit signalChangeValuePower(value);

//    _map_controller->get_map_client(1)->set_radius(_radius);
}

void NIIPPControl::_slot_change_angel(double value)
{
//    QByteArray ba;
//    QDataStream ds(&ba, QIODevice::WriteOnly);

//    ds << _name;
//    ds << _latlon;
//    ds << _width_angle;
//    _map_controller->get_map_client(1)->slot_update_sector(_id, _radius_sector, value, ba);
    _angel = value;
}

//void NIIPPControl::_slot_change_antena(int value)
//{
//}

void NIIPPControl::set_power(double value)
{
}

void NIIPPControl::set_switch_on(bool state)
{
}

void NIIPPControl::set_antenna_type(int value)
{
    _antena_type = value;

    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);

    ds << _name;
    ds << _latlon;
    ds << _width_angle;

    if(_antena_type == 1)
    {
        _map_controller->get_map_client(1)->slot_niipp_power_cicle(_id, _radius_circle, ba);


		_mode_current_index = _control_view->getModeIndex();

    }
    else
    {
        _map_controller->get_map_client(1)->slot_update_sector(_id, _radius_sector, _angel, ba);

    }
}

int NIIPPControl::get_id()
{
    return _id;
}

void NIIPPControl::set_point(QPointF coord)
{
    _point_uvode_niipp = coord;
    QString lat_s = QString::number(coord.x(), 'f', 4);
	_control_view->setLatText(lat_s);
	QString lon_s = QString::number(coord.y(), 'f', 4);
	_control_view->setLonText(lon_s);
}

void NIIPPControl::send_evil(QPointF point, QPointF point_uvoda, double alt, double bearing)
{
	if(!_control_view->getStartState())
    {
        return;
    }

    QByteArray ba;// = new QByteArray();
    QDataStream ds(&ba, QIODevice::ReadWrite);
    QDateTime dt = QDateTime::currentDateTime();

    ds << _id;
    ds << dt.date();
    ds << dt.time();

	int index = _control_view->getAntenaIndex();
    QString str_temp;
    if(index == 1)
    {
        str_temp =  "0100";
    }
    else
    {
        switch(_mode)
        {
        case 0:
            str_temp = "0201";
            break;
        case 1:
            str_temp = "0202";
            break;
//        case 3:
//            str_temp = "0203";
//            break;
        case 2:
            str_temp = "0204";
            break;
        default:
            str_temp = "0200";
            break;
        }
    }

    ds << str_temp;

    ds << point;
    QString NS = "N";
    ds << NS;

    QString EW = "E";

    ds << EW;

    int alt1 = (int)alt;
    ds << alt1;

    int bearing1 = (int)bearing;

//    int velocity_direction = 0;
    ds << bearing1;
	int zone = _control_view->getSbPowerValue();
    ds << zone;

    ds << _point_uvode_niipp;
    QString NS2 = "N";
    ds << NS2;
    QString EW2 = "E";
    ds << EW2;

    CommandMessage* msg = new CommandMessage(COMMAND_SET_NIIPP_BPLA, ba);
    _parent_tab->send_data(0, msg);
}

void NIIPPControl::set_angle(double angle)
{
    emit signalChangeAngle(angle);
}

QByteArray NIIPPControl::_encode(QStringList list)
{
    QString str = list.join(",");
    QByteArray output = str.toLocal8Bit();
    quint8 crc = 0;
    for(int i = 0; i < output.length(); i++)\
    {
        crc ^= output.at(i);
    }
    str.append(",*");
    str.append(QString::number(crc));
    str.append("\\r\\n");

    QByteArray ba_out;
    QDataStream ds(&ba_out, QIODevice::WriteOnly);
    ds << _id;
    ds << str;
    return ba_out;
}

void NIIPPControl::_slot_change_mode(int value)
{
    _mode = value;
}

int NIIPPControl::getAntenaType()
{
	return _antena_type;
}

double NIIPPControl::getRadiusCircle()
{
	return _radius_circle;
}

double NIIPPControl::getRadiusSector()
{
	return _radius_sector;
}

int NIIPPControl::getModeCurrentIndex()
{
	return _mode_current_index;
}

QWidget* NIIPPControl::getControlWidget()
{
	return _control_view;
}
