#include "RPCClient_R.h"

#include <QDebug>

RPCClient_R::RPCClient_R(IRouter* router)
{
    _id = 701;
    _type = 10;
    _router = router;

    _subscriber = _router->get_subscriber();
    connect(this, SIGNAL(signalStart()), this, SLOT(start()));
    connect(this, SIGNAL(signalStop()), this, SLOT(stop()));
    connect(this, SIGNAL(signalFinishRPC()), this, SLOT(_close()));
    connect(this, SIGNAL(signalSendData(QSharedPointer<IMessageOld>)), this, SLOT(_slotGetData(QSharedPointer<IMessageOld>)));
}

RPCClient_R::~RPCClient_R()
{
}

void RPCClient_R::set_id(int id)
{
    _id = id;
}

int RPCClient_R::get_id()
{
    return _id;
}

void RPCClient_R::set_type(int type)
{
    _type = type;
}

int RPCClient_R::get_type()
{
    return _type;
}

void RPCClient_R::send_data(QSharedPointer<IMessageOld> msg_ptr)
{
	emit signalSendData(msg_ptr);
}

void RPCClient_R::slotInit()
{
    QString tabs_setting_file = QCoreApplication::applicationDirPath();
    tabs_setting_file.append("/RPC/RPC_R_Server.ini");
    if(_read_settings(tabs_setting_file) != 0)
    {
        return;
    }

    _rpc_client = new QxtRPCPeer();
    connect(_rpc_client, SIGNAL(connectedToServer()), this, SLOT(_slotRCPConnetion()));
    connect(_rpc_client, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(_slotErrorRPCConnection(QAbstractSocket::SocketError)));

    connect(this, SIGNAL(signalSetCommand(IMessageOld*)), this, SLOT(_slotSetCommand(IMessageOld*)));

    _rpc_client->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);

    connect(this, SIGNAL(signalReconnection()), this, SLOT(_slotReconnection()));
    connect(_rpc_client, SIGNAL(disconnectedFromServer()), this, SLOT(_slotRPCDisconnection()));

    _rpc_client->attachSignal(this, SIGNAL(signalSetSolver(QByteArray)), RPC_SLOT_SET_DATA_TO_SOLVER);
    _rpc_client->attachSignal(this, SIGNAL(signalSetSolverClear(QByteArray)), RPC_SLOT_SET_CLEAR_TO_SOLVER);
}

int RPCClient_R::start()
{
    slotInit();
    _rpc_client->connect(_ip_RPC, _port_RPC);
    return 0;
}

int RPCClient_R::stop()
{
    _rpc_client->disconnectServer();
    return 0;
}

void RPCClient_R::_close()
{
    if(_rpc_client->isClient())
    {
        _rpc_client->disconnectServer();
        delete _rpc_client;
        _rpc_client = NULL;
    }
    emit signalFinished();
}

void RPCClient_R::set_command(IMessageOld *msg)
{
    emit signalSetCommand(msg);
}

void RPCClient_R::push_msg(QByteArray msg)
{
}

void RPCClient_R::_slotSetCommand(IMessageOld *msg)
{
    _command_msg = msg;
    _form_command(_command_msg);
}

void RPCClient_R::_slotPushMsg(QByteArray msg)
{
    emit signalSetSolver(msg);
}

void RPCClient_R::_slotGetData(QSharedPointer<IMessageOld> msg_ptr)
{
    int type1 = 1;
    int id = 0;
    IMessageOld *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    QDataStream ds(*dd);
    int id1 = 0;
    ds >> id1;
    int count = 0;
    ds >> count;
    double val = 0;
    ds >> val;
    QByteArray ba11;
    ba11.append(*dd);

    switch(type1)
    {
    case SOLVER_SET:
            emit signalSetSolver(ba11);
        break;
    case SOLVER_CLEAR:
            emit signalSetSolverClear(ba11);
        break;
    default:
        return;
        break;
    }
}

void RPCClient_R::_form_command(IMessageOld *msg)
{
}

/// slot when connection complete
void RPCClient_R::_slotRCPConnetion()
{
    qDebug() << "Connection complete!";
    emit signalSetClientId(_id);
    ///server
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_DEF, this, SLOT(rpc_slot_server_send_bpla_def(QByteArray)));
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO, this, SLOT(rpc_slot_server_send_bpla_def_auto(QByteArray)));

    _rpc_client->attachSlot(RPC_SLOT_SERVER_ATLANT_DIRECTION, this, SLOT(rpc_slot_server_atlant_direction(QByteArray)));
    _rpc_client->attachSlot(RPC_SLOT_SERVER_ATLANT_POSITION, this, SLOT(rpc_slot_server_atlant_position(QByteArray)));
}

void RPCClient_R::_slotRPCDisconnection()
{
    qDebug() << "Disconnected from server!";
    emit signalReconnection();
}

/// slot if have some error while connetiting
void RPCClient_R::_slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
{
//    qDebug() << "Have error in connection ";
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
        break;
	}

	emit signalReconnection();
}

void RPCClient_R::_slotReconnection()
{
    //QIODevice *dev = _rpc_client->takeDevice();
    _rpc_client->connect(_ip_RPC, _port_RPC);
}

/// read rpc configuration from ini file
int RPCClient_R::_read_settings(QString path_to_ini_file_RPC)
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
        m_settings.beginGroup(childKey);

        _ip_RPC = m_settings.value("IP", "127.0.0.1").toString();
        _port_RPC = m_settings.value("Port", 24550).toInt();

        error = 0;
        m_settings.endGroup();
    }

    return error;
}

void RPCClient_R::slotStart()
{
    emit signalStart();
}

void RPCClient_R::slotStop()
{
    emit signalStop();
}

void RPCClient_R::slotFinish()
{
    emit signalFinishRPC();
}

void RPCClient_R::rpc_slot_server_send_bpla_def(QByteArray ba1)
{
//    qDebug() << "GOT FROM SERVER ID BPLA! = ";// << id;
    QByteArray *ba = new QByteArray();
	ba->append(ba1);

    QSharedPointer<IMessageOld> msg(new MessageOld(_id, ARM_R_SERVER_BPLA_COORDS, ba));
    _subscriber->data_ready(ARM_R_SERVER_BPLA_COORDS, msg);
}

void RPCClient_R::rpc_slot_server_send_bpla_def_auto(QByteArray ba)
{
//    qDebug() << "GOT FROM SERVER ID BPLA! = ";// << id;

    QByteArray *ba1 = new QByteArray();
    ba1->append(ba);

    QSharedPointer<IMessageOld> msg(new MessageOld(_id, ARM_R_SERVER_BPLA_COORDS_AUTO, ba1));
    _subscriber->data_ready(ARM_R_SERVER_BPLA_COORDS_AUTO, msg);

}

void RPCClient_R::rpc_slot_server_atlant_direction(QByteArray ba1)
{
    QByteArray *ba = new QByteArray();
    ba->append(ba1);
//    qDebug() << "GOT FROM SERVER ATLANT! = ";// << id;

    QSharedPointer<IMessageOld> msg1(new MessageOld(_id, ARM_R_SERVER_ATLANT_DIRECTION, ba));
    _subscriber->data_ready(ARM_R_SERVER_ATLANT_DIRECTION, msg1);

}

void RPCClient_R::rpc_slot_server_atlant_position(QByteArray ba1)
{
    QByteArray *ba = new QByteArray();
    ba->append(ba1);

    QSharedPointer<IMessageOld> msg1(new MessageOld(_id, ARM_R_SERVER_ATLANT_POSITION, ba));
    _subscriber->data_ready(ARM_R_SERVER_ATLANT_POSITION, msg1);

}
