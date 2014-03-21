#include "NIIPPControl.h"

#include <QDebug>

const double m_zone[24] = {1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5,
                       5, 6, 7, 8, 9, 10, 11, 12, 14, 16,
                       18, 20, 22, 24, 28, 30};
const double m_zoneDir[28] = {2.5, 3, 4, 5,
                       6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20,
                       22, 26, 29, 33, 37, 41, 47, 52, 57, 62, 68, 72, 76};

NIIPPControl::NIIPPControl(int id, QString name, QPointF latlon, IRouter *router, MapController* map_controller, ITabManager* parent_tab):
    _number_command("0200"),
    _antena_type(0),
    _width_angle(25),
    _mode_current_index(-1)
{
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

    _pb_start = new QPushButton(tr("Пуск"));
    _pb_start->setStyleSheet("background-color: red;" \
                             "border-style: solid;" \
                             "border-width:1px;" \
                             "border-radius:25px;" \
                             "border-color: red;" \
                             "max-width:50px;" \
                             "max-height:50px;" \
                             "min-width:50px;"\
                             "min-height:50px");
    _pb_start->setCheckable(true);

    connect(_pb_start, SIGNAL(clicked(bool)), this, SLOT(_slot_start_stop_clicked(bool)));
    _sb_power = new QSpinBox();
    QFont* font = new QFont();
    font->setBold(true);
    font->setPixelSize(20);

    _sb_power->setFont(*font);
    _sb_power->show();

    _le_distance = new QLineEdit();
    _le_distance->setFont(*font);
    _le_distance->setText(tr("0 км"));
    _le_distance->setFixedWidth(70);
    _le_distance->show();


    QHBoxLayout* hbox = new QHBoxLayout();

    QVBoxLayout* v1 = new QVBoxLayout();
    QLabel* lbl_power = new QLabel(tr("Зона"));
    v1->addWidget(lbl_power);
    v1->addWidget(_sb_power);
    hbox->addLayout(v1);

    QVBoxLayout* v2 = new QVBoxLayout();
    QLabel* lbl_distance = new QLabel(tr("Радиус"));
    v2->addWidget(lbl_distance);
    v2->addWidget(_le_distance);
    hbox->addLayout(v2);
    hbox->addWidget(_pb_start);

    _sl_power = new QSlider(Qt::Horizontal);
    _sl_power->setRange(0, 27);
    _sl_power->setTickPosition(QSlider::TicksBelow);
    _sl_power->setTickInterval(4);

    connect(_sl_power, SIGNAL(valueChanged(int)), _sb_power, SLOT(setValue(int)));
//    connect(_sb_power, SIGNAL(valueChanged(int)), _sl_power, SLOT(setValue(int)));

    connect(_sb_power, SIGNAL(valueChanged(int)), this, SLOT(_slot_change_value_power(int)));


    QVBoxLayout* vbox = new QVBoxLayout();

    _pb_enable_complex = new QPushButton(tr("Включить комплекс"));
    _pb_enable_complex->setCheckable(true);
    _pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
    connect(_pb_enable_complex, SIGNAL(toggled(bool)), this, SLOT(_slot_enable_complex(bool)));
    vbox->addWidget(_pb_enable_complex);


    vbox->addLayout(hbox);
    vbox->addWidget(_sl_power);

    QLabel* lbl_antena = new QLabel(tr("Выбор антенны"));
    _cb_antena = new QComboBox();
    _cb_antena->addItem(tr("Направленная"));
    _cb_antena->addItem(tr("Ненапрвленная"));
    QVBoxLayout* antena_layout = new QVBoxLayout();
    antena_layout->addWidget(lbl_antena);
    antena_layout->addWidget(_cb_antena);

    connect(_cb_antena, SIGNAL(activated(int)), this, SLOT(set_antenna_type(int)));

    QLabel* lbl_mode = new QLabel(tr("Режим работы"));
    _cb_mode = new QComboBox();
    _cb_mode->addItem(tr("Облучение"));
    _cb_mode->addItem(tr("Приведение"));
//    _cb_mode->addItem(tr("ОД"));
    _cb_mode->addItem(tr("Круговой обзор"));
    QVBoxLayout* mode_layout = new QVBoxLayout();
    mode_layout->addWidget(lbl_mode);
    mode_layout->addWidget(_cb_mode);

    connect(_cb_mode, SIGNAL(activated(int)), this, SLOT(_slot_change_mode(int)));

    QHBoxLayout* hbox3 = new QHBoxLayout();
    hbox3->addLayout(antena_layout);
    hbox3->addLayout(mode_layout);
    vbox->addLayout(hbox3);

    QHBoxLayout* hbox_l1 = new QHBoxLayout();
    QVBoxLayout* point_layout_lat = new QVBoxLayout();

    QLabel* lbl_l1 = new QLabel(tr("Широта"));
    point_layout_lat->addWidget(lbl_l1);
    _le_lat = new QLineEdit();
    _le_lat->setFixedWidth(70);

    point_layout_lat->addWidget(_le_lat);

    hbox_l1->addLayout(point_layout_lat);

    QVBoxLayout* point_layout_lon = new QVBoxLayout();

    QLabel* lbl_l2 = new QLabel(tr("Долгота"));
    point_layout_lon->addWidget(lbl_l2);
    _le_lon = new QLineEdit();
    _le_lon->setFixedWidth(70);
    point_layout_lon->addWidget(_le_lon);

    hbox_l1->addLayout(point_layout_lon);

    QVBoxLayout* point_layout_clear = new QVBoxLayout();

    QLabel* lbl_l3 = new QLabel(tr(""));
    point_layout_clear->addWidget(lbl_l3);
    _clear_uvod = new QPushButton(tr("Очистить"));
    point_layout_clear->addWidget(_clear_uvod);
    hbox_l1->addLayout(point_layout_clear);

    connect(_clear_uvod, SIGNAL(clicked()), this, SLOT(_slot_clear()));
    vbox->addLayout(hbox_l1);



    _le_status = new QLineEdit();
    _le_status->setReadOnly(true);
    _le_status->setText(tr("Простой"));
    QLabel* lbl_status = new QLabel(tr("Статус"));

    QVBoxLayout* v3 = new QVBoxLayout();
    v3->addWidget(lbl_status);
    v3->addWidget(_le_status);

    vbox->addLayout(v3);

//    _sl_angel = new QSlider(Qt::Horizontal);
//    _sl_angel->setRange(0, 360);
//    _sl_angel->setTickPosition(QSlider::TicksBelow);
//    _sl_angel->setTickInterval(45);

//    vbox->addWidget(_sl_angel);

    this->setLayout(vbox);

    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(p);
    this->show();

    create();


//    connect(_sl_angel, SIGNAL(valueChanged(int)), this, SLOT(_slot_change_angel(int)));

//    NIIPPParser *parser = new NIIPPParser();
//    parser->set_router(_router);

//    _router->get_tcp_controller()->add_connection(_ip, _port, parser, 0);
}

NIIPPControl::~NIIPPControl()
{
    delete _sb_power;
    delete _pb_start;
    delete _sl_power;
    delete _le_distance;
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
    return _pb_enable_complex->isChecked();
}

void NIIPPControl::set_data(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);

    int mode;

    ds >> mode;
    switch(mode)
    {
    case 00:
        _le_status->setText(tr("Простой"));
        break;
    case 01:
        _le_status->setText(tr("Облучение"));
        break;
    case 10:
        _le_status->setText(tr("Облучение"));
        break;
    case 11:
        _le_status->setText(tr("Авария"));
        break;
    case 55:
        _le_status->setText(tr("Нет координат"));
        break;
    case 99:
        _le_status->setText(tr("Обслуживание"));
        break;
    default:
        _le_status->setText(tr("Нет данных"));
        break;
    }

}

void NIIPPControl::_slot_start_stop_clicked(bool state)
{
    _pb_start->setChecked(state);

    if(state == true)
    {
        _pb_start->setStyleSheet("background-color: green;" \
                                 "border-style: solid;" \
                                 "border-width:1px;" \
                                 "border-radius:25px;" \
                                 "border-color: red;" \
                                 "max-width:50px;" \
                                 "max-height:50px;" \
                                 "min-width:50px;"\
                                 "min-height:50px");
    }
    else
    {
        _pb_start->setStyleSheet("background-color: red;" \
                                 "border-style: solid;" \
                                 "border-width:1px;" \
                                 "border-radius:25px;" \
                                 "border-color: red;" \
                                 "max-width:50px;" \
                                 "max-height:50px;" \
                                 "min-width:50px;"\
                                 "min-height:50px");

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
        _pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(0,255,0);"));
    }
    else
    {
        status = "0001";
        _pb_enable_complex->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
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
    _le_lat->clear();
    _le_lon->clear();

    _map_controller->get_map_client(1)->slot_remove_point_uvoda();
}

void NIIPPControl::_slot_change_value_power(int value)
{
    _sl_power->setValue(value);

    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);

    ds << _name;
    ds << _latlon;
    ds << _width_angle;


    if(_antena_type == 0)
    {
		_map_controller->get_map_client(1)->slot_update_sector(_id, m_zoneDir[value], _angel, ba);
		_radius_sector = m_zoneDir[value];
        _le_distance->setText(QString("%1 км").arg(QString::number(_radius_sector)));
        if(_pb_start->isChecked())
        {
            _le_status->setText(tr("Облучение"));
        }
    }
    if(_antena_type == 1)
    {
		_map_controller->get_map_client(1)->slot_niipp_power_cicle(_id, m_zone[value], ba);
		_radius_circle = m_zone[value];
        _le_distance->setText(QString("%1 км").arg(QString::number(_radius_circle)));
        if(_pb_start->isChecked())
        {
            _le_status->setText(tr("Облучение"));
        }
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

        _cb_mode->setEnabled(false);

        _mode_current_index = _cb_mode->currentIndex();
        _cb_mode->setCurrentIndex(-1);

        _sl_power->setRange(0, 23);
        _sl_power->update();
        _sb_power->setRange(0, 23);
        _sb_power->update();
        if(_pb_start->isChecked())
        {
            _le_status->setText(tr("Облучение"));
        }

    }
    else
    {
        _map_controller->get_map_client(1)->slot_update_sector(_id, _radius_sector, _angel, ba);
        _cb_mode->setEnabled(true);

        _cb_mode->setCurrentIndex(_mode_current_index);

        _sl_power->setRange(0, 27);
        _sl_power->update();
        _sb_power->setRange(0, 27);
        _sb_power->update();
        if(_pb_start->isChecked())
        {
            _le_status->setText(tr("Облучение"));
        }

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
    _le_lat->setText(lat_s);
    QString lon_s = QString::number(coord.y(), 'f', 4);
    _le_lon->setText(lon_s);
}

void NIIPPControl::send_evil(QPointF point, QPointF point_uvoda, double alt, double bearing)
{
    if(!_pb_start->isChecked())
    {
        return;
    }

    QByteArray ba;// = new QByteArray();
    QDataStream ds(&ba, QIODevice::ReadWrite);
    QDateTime dt = QDateTime::currentDateTime();

    ds << _id;
    ds << dt.date();
    ds << dt.time();

    int index = _cb_antena->currentIndex();
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
    int zone = _sb_power->value();
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
