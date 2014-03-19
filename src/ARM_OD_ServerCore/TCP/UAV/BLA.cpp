#include "BLA.h"

BLA::BLA(QString ip, int port, int id, int dev, IRouter *router)
{
    _ip = ip;
    _port = port;
    _id = id;
    _dev = dev;
    _router = router;
    _tcp_controller = _router->get_tcp_controller();
    _parser = new BLAParser(_id, _router);
    _parser2 = new BLAParser2(_id*100, _router);
//    IController *contr = _router->get_models_controller(1);
//    QStringList list;
//    list.push_back(QString::number(_id));
//    list.push_back("0");
//    list.push_back("0");
//    list.push_back("0");
//    list.push_back("0");
//    list.push_back("0");
//    list.push_back("0");
//    list.push_back("0");
//    list.push_back("0");
//    list.push_back("1");
//    contr->add_new_bla(list);
}

BLA::~BLA()
{
    delete _parser;
}

void BLA::set()
{
    _tcp_controller->add_connection(_ip, _port, _id);
    ITCPClientOld *client1 = _tcp_controller->get_client(_ip, _port, _id);
    client1->set_parser(_parser);
    connect(client1, SIGNAL(signalConnected(bool)), this, SLOT(_send_request(bool)));

    _tcp_controller->add_connection(_ip, _port, _id*100);
    ITCPClientOld *client2 = _tcp_controller->get_client(_ip, _port, _id*100);
    client2->set_parser(_parser2);
    connect(client2, SIGNAL(signalConnected(bool)), this, SLOT(_send_request2(bool)));

}

void BLA::_send_request(bool state)
{
    if(!state)
        return;

    ITCPClientOld *client1 = _tcp_controller->get_client(_ip, _port, _id);
    QString request = "user link command b" + QString::number(_id) + "d" +QString::number(_dev);
    QByteArray ba(request.toAscii());
    client1->set(ba);
}

void BLA::_send_request2(bool state)
{
    if(!state)
        return;

    ITCPClientOld *client1 = _tcp_controller->get_client(_ip, _port, _id*100);
    QString request1 = "user link command b" + QString::number(_id) + "d" +QString::number(622);
    QByteArray ba1(request1.toAscii());
    client1->set(ba1);
}
