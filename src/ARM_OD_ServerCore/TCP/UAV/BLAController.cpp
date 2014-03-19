#include "BLAController.h"
#include <QDebug>

BLAController::BLAController(IRouter *router) :
    _id(0),
    _ip("192.168.1.33"),
    _port(64300),
    _dev_control_BLA(1)
{
    _parser = NULL;
    _router = router;
    _tcp_controller = _router->get_tcp_controller();
    _parser = new BLAControllerParser(this);

}

BLAController::~BLAController()
{
    delete _parser;
}

void BLAController::setController(QString ip, quint16 port)
{
    TCPClientOld *client = _tcp_controller->get_client(_ip, _port, _id);
    if(client)
        client->slotDestroy();

    _ip = ip;
    _port = port;

    qDebug() << _tcp_controller->add_connection(_ip, _port, _id);

    TCPClientOld *client1 = _tcp_controller->get_client(_ip, _port, _id);
    client1->set_parser(_parser);
    connect(client1, SIGNAL(signalConnected(bool)), this, SLOT(_send_request(bool)));
}

void BLAController::_send_request(bool state)
{
    if(!state)
        return;

    ITCPClientOld *client1 = _tcp_controller->get_client(_ip, _port, _id);
    QString request = "user link command ctrl";
    QByteArray ba(request.toAscii());
    client1->set(ba);
}

/// interface method for looking for bla connections
void BLAController::connect_bla(QVector<quint16> ids)
{
    QMap<quint16, ITCPClientOld *>::iterator it;
    for(it = _map_clients.begin(); it != _map_clients.end(); ++it)
    {
        if(!ids.contains(it.key()))
        {
            it.value()->slotDestroy();
        }
        else
        {
            qDebug() << it.key();
            int index = ids.indexOf(it.key());
            ids.remove(index);
        }
    }
    if(ids.isEmpty())
        return;

    QVector<quint16>::iterator itt;
    for(itt = ids.begin(); itt != ids.end(); ++itt)
    {
        int id = *itt;
        BLA *bla = new BLA(_ip, _port, id, _dev_control_BLA, _router);
        bla->set();
        ITCPClientOld *client1 = _tcp_controller->get_client(_ip, _port, _id);

        _map_clients.insert(id, client1);
    }
}
