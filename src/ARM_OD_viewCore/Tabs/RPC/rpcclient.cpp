#include "RPCClient.h"

#include <QDebug>

const double _zone[24] = {1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5,
                       5, 6, 7, 8, 9, 10, 11, 12, 14, 16,
                       18, 20, 22, 24, 28, 30};
const double _zone_dir[28] = {2.5, 3, 4, 5,
                       6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20,
                       22, 26, 29, 33, 37, 41, 47, 52, 57, 62, 68, 72, 76};

RPCClient::RPCClient(TabsProperty *prop, IDBManager *db_manager, IDBManager* db_manager_targer, IMapController* map_controller, ITabMap* parent_tab, ITabManager* tab_manager)
{
    _tab_manager = tab_manager;
    _solver_auto = false;
    _map_controller = map_controller;
    _parent_tab = parent_tab;
    _tab_property = prop;
    _db_manager = db_manager;
    _db_manager_target = db_manager_targer;
    _peleng_evil_ids = 0;
    _rds_evil_ids = 50;

    connect(this, SIGNAL(signalStart()), this, SLOT(start()));
    connect(this, SIGNAL(signalStop()), this, SLOT(stop()));
    connect(this, SIGNAL(signalFinishRPC()), this, SLOT(_close()));
}

RPCClient::~RPCClient()
{
//    _close();
//    delete _rpc_client;
}

void RPCClient::slotInit()
{
    QString tabs_setting_file = QCoreApplication::applicationDirPath();
    tabs_setting_file.append("/Tabs/RPC.ini");
    if(_read_settings(tabs_setting_file) != 0)
    {
        return;
    }

    _rpc_client = new QxtRPCPeer();
    connect(_rpc_client, SIGNAL(connectedToServer()), this, SLOT(_slotRCPConnetion()));
    connect(_rpc_client, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(_slotErrorRPCConnection(QAbstractSocket::SocketError)));

    connect(this, SIGNAL(signalSetCommand(IMessage*)), this, SLOT(_slotSetCommand(IMessage*)));

    _rpc_client->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
    _rpc_client->attachSignal(this, SIGNAL(signalSetNIIPPBPLA(QByteArray)), RPC_SLOT_SET_NIIPP_BPLA);
    _rpc_client->attachSignal(this, SIGNAL(signalSetSolverData(QByteArray)), RPC_SLOT_SET_SOLVER_DATA);
    _rpc_client->attachSignal(this, SIGNAL(signalSetSolverDataClear(QByteArray)), RPC_SLOT_SET_SOLVER_CLEAR);


    connect(this, SIGNAL(signalReconnection()), this, SLOT(_slotReconnection()));
    connect(_rpc_client, SIGNAL(disconnectedFromServer()), this, SLOT(_slotRPCDisconnection()));
    qDebug() << this->thread();
}

int RPCClient::start()
{
    qDebug() << this->thread();
    _rpc_client->connect(_ip_RPC, _port_RPC);
    return 0;
}

int RPCClient::stop()
{
    _rpc_client->disconnectServer();
    return 0;
}

void RPCClient::_close()
{
    if(_rpc_client->isClient())
    {
        _rpc_client->disconnectServer();
        delete _rpc_client;
        _rpc_client = NULL;
    }
    emit signalFinished();
}

void RPCClient::set_command(IMessage *msg)
{
    emit signalSetCommand(msg);
}

void RPCClient::_slotSetCommand(IMessage *msg)
{
    _command_msg = msg;
    _form_command(_command_msg);
}

void RPCClient::_form_command(IMessage *msg)
{
    qDebug() << "form command";
    QByteArray data;
    int type = msg->get(data);
    switch(type)
    {
    case COMMAND_SET_NIIPP_BPLA:
        _send_niipp_bpla(data);
        break;

    case COMMAND_SET_SOLVER:
        _send_data_to_sovler(data);
        break;
    case COMMAND_SET_SOLVER_AUTO:
        _set_solver_auto(data);
        break;

    case COMMAND_SET_SOLVER_CLEAR:
        _set_solver_clear(data);
        break;


//    case KTR_BLA:

//        rpc_slot_server_send_bla_points(int id, rpc_QPointF point, double alt);
//        break;
//    case COMMAND_TOBLACKLIST:
//        break;
//    case COMMAND_TOWHITELIST:
//        break;
//    case COMMAND_RECOGNIZESIGNAL:
//        _recognize();
//        break;
//    case COMMAND_KM:
//        _ss_correlation(data.toBool());
//        break;
//    case COMMAND_PRM_SET_FREQ:
//        _prm_set_freq(data.toUInt());
//        break;
//    case COMMAND_PRM_REQUEST_FREQ:
//        _prm_request_freq();
//        break;
//    case COMMAND_PRM_SET_ATT1:
//        _prm_set_att1(data.toInt());
//        break;
//    case COMMAND_PRM_SET_ATT2:
//        _prm_set_att2(data.toInt());
//        break;
//    case COMMAND_PRM_SET_FILTER:
//        _prm_set_filter(data.toInt());
//        break;
    default:
        break;
    }
    msg->clenup();
}

void RPCClient::_set_solver_auto(QByteArray ba)
{
    QDataStream ds(&ba, QIODevice::ReadOnly);
    ds >> _solver_auto;
}

/// slot when connection complete
void RPCClient::_slotRCPConnetion()
{
    qDebug() << "Connection complete!";
    emit signalSetClientId(_tab_property->get_id());
    ///server
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_BLA_POINTS, this, SLOT(rpc_send_bla_points(int,QPointF,double,double,double,int)));
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_AIS_DATA, this, SLOT(rpc_slot_server_send_ais_data(QByteArray)));
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_POINTS, this, SLOT(rpc_send_bpla_points(QByteArray)));
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO, this, SLOT(rpc_send_bpla_points_auto(QByteArray)));

    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_NIIPP_DATA, this, SLOT(rpc_send_niipp_data(QByteArray)));
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION, this, SLOT(rpc_slot_server_send_atlant_direction(QByteArray)));
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_ATLANT_POSITION, this, SLOT(rpc_slot_server_send_atlant_position(QByteArray)));

    //    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_POINTS, this, SLOT(rpc_slot_getting_points(rpc_send_points_vector)));
//    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, this, SLOT(rpc_slot_getting_modulation(QString)));
//    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_CORRELATION, this, SLOT(rpc_slot_server_send_correlation(int, int, rpc_send_points_vector)));

//    _rpc_client->attachSlot(RPC_SLOT_SERVER_PRM_STATUS, this, SLOT(rpc_slot_server_prm_status(int, int, int, int)));

//    CommandMessage *msg = new CommandMessage(COMMAND_PRM_REQUEST_FREQ, QVariant());
//    this->set_command(msg);
}

void RPCClient::_slotRPCDisconnection()
{
    qDebug() << "Disconnected from server!";
    emit signalReconnection();
}

/// slot if have some error while connetiting
void RPCClient::_slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Have error in connection ";
    QString thiserror;
    switch(socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        thiserror.append(("Ошибка! Соеденение с пунктом потеряно!"));
        //return;
        break;
    case QAbstractSocket::HostNotFoundError:
        thiserror.append(("Ошибка! Не удалось подключиться к пункту!"));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        thiserror.append(("Ошибка! Отказано в соединении"));
        break;
    default:
//        thiserror.append(("Ошибка! Произошла ошибка: " + _rpc_client->->errorString()));
        break;
    }
//    _rpc_client->takeDevice()->;

    emit signalReconnection();
    qDebug() << thiserror;
}

void RPCClient::_slotReconnection()
{
    QIODevice *dev = _rpc_client->takeDevice();
    _rpc_client->connect(_ip_RPC, _port_RPC);
}

/// read rpc configuration from ini file
int RPCClient::_read_settings(QString path_to_ini_file_RPC)
{
    int error = -1;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QSettings m_settings(path_to_ini_file_RPC, QSettings::IniFormat);

    m_settings.setIniCodec(codec);

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
        if(childKey.toLatin1() != "RPC_UI")
        {
            continue;
        }
        qDebug() << "m_settings = " << childKey.toLatin1();
        m_settings.beginGroup(childKey);

        _ip_RPC = m_settings.value("IP", "127.0.0.1").toString();
        _port_RPC = m_settings.value("Port", 24500).toInt();

        error = 0;
        m_settings.endGroup();
    }

    return error;
}

void RPCClient::slotStart()
{
    emit signalStart();
}

void RPCClient::slotStop()
{
    emit signalStop();
}

void RPCClient::slotFinish()
{
    emit signalFinishRPC();
}

void RPCClient::rpc_send_bla_points(int id, QPointF point, double alt, double speed, double course, int state)
{
    QByteArray ddd;
    QDataStream ds(&ddd, QIODevice::WriteOnly);
    ds << point;
    ds << alt;
    ds << speed;
    ds << course;
    ds << state;

    _map_controller->get_map_client(1)->slot_add_BLA(id, ddd);
    QMap<QString, QVariant>* rec = new QMap<QString, QVariant>;

    rec->insert("id", QVariant::fromValue(id));

    rec->insert("pid", QVariant::fromValue(0));
    rec->insert("name", QVariant::fromValue(id));
    rec->insert("state", QVariant::fromValue(1));
    _db_manager->set(0, rec);

    QMap<QString, QVariant>* rec_p = new QMap<QString, QVariant>;

    QString s_prop;
    s_prop = tr("Широта");
    rec_p->insert("pid", QVariant::fromValue(id));
    rec_p->insert("name", QVariant::fromValue(s_prop));
    rec_p->insert("value", QVariant::fromValue(point.x()));
    rec_p->insert("state", QVariant::fromValue(1));


//    rec_p->insert("id", QVariant::fromValue(-1));

    QVector<QMap<QString, QVariant> >* map_p = _db_manager->get(id, 0);
    for(int i = 0; i < map_p->count(); ++i)
    {
        QString nam = map_p->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p->insert("id", QVariant::fromValue(map_p->at(i).value("id").toInt()));
            break;
        }
    }
//    qDebug() << map_p->value("name").toString() << s_prop;
//    qDebug() << QString::compare((map_p->value("name")).toString(), s_prop);
//    if(QString::compare((map_p->value("name")).toString(), s_prop) == 0)
//    {
//        rec_p->insert("id", QVariant::fromValue(map_p->value("id").toInt()));
//    }

    _db_manager->set_property(0, rec_p);

    QMap<QString, QVariant>* rec_p1 = new QMap<QString, QVariant>;

    s_prop = tr("Долгота");
    rec_p1->insert("pid", QVariant::fromValue(id));
    rec_p1->insert("name", QVariant::fromValue(s_prop));
    rec_p1->insert("value", QVariant::fromValue(point.y()));
    rec_p1->insert("state", QVariant::fromValue(1));

//    map_p = _db_manager->get(id, 0);
//    if(QString::compare((map_p->value("name")).toString(), s_prop) == 0)
//    {
//        rec_p->insert("id", QVariant::fromValue(map_p->value("id").toInt()));
//    }

    QVector<QMap<QString, QVariant> >* map_p1 = _db_manager->get(id, 0);

//    rec_p1->insert("id", QVariant::fromValue(-1));

    for(int i = 0; i < map_p1->count(); ++i)
    {
        QString nam = map_p1->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p1->insert("id", QVariant::fromValue(map_p1->at(i).value("id").toInt()));
            break;
        }
    }

    _db_manager->set_property(0, rec_p1);


    QMap<QString, QVariant>* rec_p2 = new QMap<QString, QVariant>;

    s_prop = tr("Высота");
    rec_p2->insert("pid", QVariant::fromValue(id));
    rec_p2->insert("name", QVariant::fromValue(s_prop));
    rec_p2->insert("value", QVariant::fromValue(alt));
    rec_p2->insert("state", QVariant::fromValue(1));

//    map_p = _db_manager->get(id, 0);
//    if(QString::compare((map_p->value("name")).toString(), s_prop) == 0)
//    {
//        rec_p->insert("id", QVariant::fromValue(map_p->value("id").toInt()));
//    }

//    rec_p2->insert("id", QVariant::fromValue(-1));

    QVector<QMap<QString, QVariant> >* map_p2 = _db_manager->get(id, 0);

    for(int i = 0; i < map_p2->count(); ++i)
    {
        QString nam = map_p2->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p2->insert("id", QVariant::fromValue(map_p2->at(i).value("id").toInt()));
            break;
        }
    }

    _db_manager->set_property(0, rec_p2);
}

void RPCClient::rpc_slot_server_send_ais_data(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);
    int id;
    ds >> id;
    QMap<int, QVector<QString> > map1;
//    rpc_data_fly vec;
    ds >> map1;

    _map_controller->get_map_client(1)->slot_add_ais(map1);

}

void RPCClient::rpc_send_bpla_points(QByteArray data)
{
    if(_solver_auto == false)
    {
        _send_bpla_points(data);
    }

}

void RPCClient::rpc_send_bpla_points_auto(QByteArray data)
{
    if(_solver_auto == true)
    {
        _send_bpla_points(data);
    }

}

void RPCClient::rpc_send_niipp_data(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);

    QDateTime dt;
    QTime time;
    int mode;
    QPointF point;
    QString NS;
    QString EW;
    int alt;
    int zone;
    int course;
    int angle;

    int id;

    ds >> id;

    ds >> dt;
    ds >> time;
    ds >> mode;
    ds >> point;
    ds >> NS;
    ds >> EW;
    ds >> alt;
    ds >> zone;
    ds >> course;
    ds >> angle;

//    qDebug() << dt;
//    qDebug() << time;
//    qDebug() << mode;
//    qDebug() << point;
//    qDebug() << NS;
//    qDebug() << EW;
//    qDebug() << alt;
//    qDebug() << zone;
//    qDebug() << course;
//    qDebug() << angle;

    QByteArray ba;
    QDataStream ds1(&ba, QIODevice::WriteOnly);

    QPointF latlon;
    if(id == 100)
    {
        latlon.setX(42.511183);
        latlon.setY(41.6905);
    }
    if(id ==101)
    {
        latlon.setX(42.634183);
        latlon.setY(41.912167);
    }

    QString name = QString::number(id) + " - niipp";
    double wid = 25;
    ds1 << name;
    ds1 << latlon;
    ds1 << wid;




    if(mode == 01)
    {
        _map_controller->get_map_client(1)->slot_niipp_power_cicle(id, _zone[zone], ba);
    }

    if(mode == 10)
    {
        _map_controller->get_map_client(1)->slot_update_sector(id, _zone_dir[zone], course, NULL);
    }

        qDebug() << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMM = " << mode << course << zone;

    QByteArray ba1;
    QDataStream ds2(&ba1, QIODevice::WriteOnly);
    ds2 << mode;
    _tab_manager->send_data_niipp_control(id, ba1);

}

void RPCClient::rpc_slot_server_send_atlant_direction(QByteArray data)
{
//    qDebug() << "GOT DATA FROM ATLANT!";
    QDataStream ds(&data, QIODevice::ReadWrite);
    A_Dir_Ans_msg msg;
    ds >> msg.requestId;
    ds >> msg.sourceId;
    ds >> msg.dateTime;
    ds >> msg.post;
    ds >> msg.postLatitude;
    ds >> msg.postLongitude;
    ds >> msg.postHeight;
    ds >> msg.frequency;
    ds >> msg.widht;
    ds >> msg.direction;
    ds >> msg.angle;
    ds >> msg.level;
    ds >> msg.quality;
    ds >> msg.motionType;
    ds >> msg.motionConfidence;

    int id_post = msg.post.right(1).toInt();

    _map_controller->get_map_client(1)->slot_peleng(msg.sourceId, id_post, msg.postLatitude, msg.postLongitude, msg.direction);
}

void RPCClient::rpc_slot_server_send_atlant_position(QByteArray data)
{
//    qDebug() << "GOT DATA FROM ATLANT!";
    QDataStream ds(&data, QIODevice::ReadWrite);
    A_Pos_Ans_msg msg;
    ds >> msg.requestId;
    ds >> msg.sourceId;
    ds >> msg.dateTime;
    ds >> msg.latitude;
    ds >> msg.longitude;
    ds >> msg.quality;

    if(!_map_peleng_evil_ids.contains(msg.sourceId))
    {
        _map_peleng_evil_ids.insert(msg.sourceId, ++_peleng_evil_ids);
        if(_peleng_evil_ids > 49)
        {
            _peleng_evil_ids = 0;
            _map_peleng_evil_ids.clear();
        }
    }
    QPointF point(msg.longitude, msg.latitude);

    QByteArray ba1;
    QDataStream ds1(&ba1, QIODevice::WriteOnly);
    ds1 << point;

    qDebug() << "ID = " << _map_peleng_evil_ids.size() << _peleng_evil_ids << _map_peleng_evil_ids.value(msg.sourceId);

    _map_controller->get_map_client(1)->slot_add_evil(_map_peleng_evil_ids.value(msg.sourceId), ba1);

}

void RPCClient::_send_niipp_bpla(QByteArray data)
{
    emit signalSetNIIPPBPLA(data);
}



void RPCClient::_set_solver_clear(QByteArray data)
{
    emit signalSetSolverDataClear(data);
}

void RPCClient::_send_data_to_sovler(QByteArray data)
{
    emit signalSetSolverData(data);
}

void RPCClient::_send_bpla_points(QByteArray data)
{
    QDataStream ds(&data, QIODevice::ReadOnly);
    QTime t;
    int state;
    QPointF pointStdDev;
    double speed;
    double alt;
    double course;
    QVector<QPointF> track;
    ds >> t;
    ds >> state;
    ds >> pointStdDev;
    ds >> track;
    ds >> speed;
    ds >> alt;
    ds >> course;

    if(_rds_evil_ids > 99)
    {
        _rds_evil_ids = 50;
    }
    _map_controller->get_map_client(1)->slot_add_evil(_rds_evil_ids, data);

    QMap<QString, QVariant>* rec = new QMap<QString, QVariant>;

    rec = _db_manager_target->get_bpla_fields(_rds_evil_ids);
    if(rec->count() == 0)
    {
        rec->insert("id", QVariant::fromValue(_rds_evil_ids));
        rec->insert("pid", QVariant::fromValue(0));
        rec->insert("name", QVariant::fromValue(_rds_evil_ids));
        rec->insert("state", QVariant::fromValue(0));
        _db_manager_target->set(1, rec);
    }


    //    qDebug() << 666;

    QMap<QString, QVariant>* rec_p = new QMap<QString, QVariant>;

    QString s_prop;
    s_prop = tr("Широта");
    rec_p->insert("pid", QVariant::fromValue(_rds_evil_ids));
    rec_p->insert("name", QVariant::fromValue(s_prop));
    rec_p->insert("value", QVariant::fromValue(track.at(track.size()-1).x()));
    rec_p->insert("state", QVariant::fromValue(1));

//    rec_p->insert("id", QVariant::fromValue(1));

    QVector<QMap<QString, QVariant> >* map_p = _db_manager_target->get(_rds_evil_ids, 1);
    for(int i = 0; i < map_p->count(); ++i)
    {
        QString nam = map_p->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p->insert("id", QVariant::fromValue(map_p->at(i).value("id").toInt()));
            break;
        }
    }
//    qDebug() << map_p->value("name").toString() << s_prop;
//    qDebug() << QString::compare((map_p->value("name")).toString(), s_prop);
//    if(QString::compare((map_p->value("name")).toString(), s_prop) == 0)
//    {
//        rec_p->insert("id", QVariant::fromValue(map_p->value("id").toInt()));
//    }

    _db_manager_target->set_property(1, rec_p);

    QMap<QString, QVariant>* rec_p1 = new QMap<QString, QVariant>;

    s_prop = tr("Долгота");
    rec_p1->insert("pid", QVariant::fromValue(_rds_evil_ids));
    rec_p1->insert("name", QVariant::fromValue(s_prop));
    rec_p1->insert("value", QVariant::fromValue(track.at(track.size()-1).y()));
    rec_p1->insert("state", QVariant::fromValue(1));

//    map_p = _db_manager->get(id, 0);
//    if(QString::compare((map_p->value("name")).toString(), s_prop) == 0)
//    {
//        rec_p->insert("id", QVariant::fromValue(map_p->value("id").toInt()));
//    }

//    rec_p1->insert("id", QVariant::fromValue(2));

    QVector<QMap<QString, QVariant> >* map_p1 = _db_manager->get(_rds_evil_ids, 1);

    for(int i = 0; i < map_p1->count(); ++i)
    {
        QString nam = map_p1->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p1->insert("id", QVariant::fromValue(map_p1->at(i).value("id").toInt()));
            break;
        }
    }

    _db_manager_target->set_property(1, rec_p1);


    QMap<QString, QVariant>* rec_p2 = new QMap<QString, QVariant>;

    s_prop = tr("Высота");
    rec_p2->insert("pid", QVariant::fromValue(_rds_evil_ids));
    rec_p2->insert("name", QVariant::fromValue(s_prop));
    rec_p2->insert("value", QVariant::fromValue(alt));
    rec_p2->insert("state", QVariant::fromValue(1));

//    map_p = _db_manager->get(id, 0);
//    if(QString::compare((map_p->value("name")).toString(), s_prop) == 0)
//    {
//        rec_p->insert("id", QVariant::fromValue(map_p->value("id").toInt()));
//    }

//    rec_p2->insert("id", QVariant::fromValue(3));

    QVector<QMap<QString, QVariant> >* map_p2 = _db_manager_target->get(_rds_evil_ids, 1);

    for(int i = 0; i < map_p2->count(); ++i)
    {
        QString nam = map_p2->at(i).value("name").toString();
        if(nam == s_prop)
        {
            rec_p2->insert("id", QVariant::fromValue(map_p2->at(i).value("id").toInt()));
            break;
        }
    }

    _db_manager_target->set_property(1, rec_p2);
}
