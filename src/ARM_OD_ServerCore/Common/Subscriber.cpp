#include "Subscriber.h"

#include <QDebug>

Subscriber::Subscriber()
{
//    qRegisterMetaType<rpc_msg> ("rpc_msg");
    connect(this, SIGNAL(signalGetData(int,rpc_msg)), this, SLOT(_slotGetData(int,rpc_msg)));
}

Subscriber::~Subscriber()
{
}

void Subscriber::add_subscription(int type, IClient *client)
{
    _mux.lock();
    QVector<IClient *> clients;
    if(!_map_subscribe.contains(type))
    {
        clients.push_back(client);
        _map_subscribe.insert(type, clients);
        _mux.unlock();
        return;
    }
    clients = _map_subscribe.value(type);
    if(clients.contains(client))
    {
        _mux.unlock();
        return;
    }

    clients.push_back(client);
    _map_subscribe.insert(type, clients);

    qDebug() << "RPC clients count = " << clients.count();
    _mux.unlock();
}

void Subscriber::remove_subscription(int type, IClient* client)
{
    _mux.lock();
    QVector<IClient *> clients = _map_subscribe.value(type);
    if(clients.contains(client))
    {
        int index = clients.indexOf(client);
        clients.remove(index);
    }
    _mux.unlock();
}

void Subscriber::remove_subscription(int type)
{
    _mux.lock();
    _map_subscribe.remove(type);
    _mux.unlock();
}

void Subscriber::remove_subscription(IClient *client)
{
    _mux.lock();
    QMap<int, QVector<IClient *> >::iterator it;
    QVector<IClient *> vec;
    IClient *cl;
    for(it = _map_subscribe.begin(); it != _map_subscribe.end(); ++it)
    {
        vec = it.value();
        for(int i = 0; i < vec.size(); i++)
        {
            cl = vec.at(i);
            if(cl == client)
            {
                vec.remove(i);
            }
        }
        if(vec.isEmpty())
        {
            //_map_subscribe.remove(it.key());
        }
//        qDebug() << vec.count();
        _map_subscribe.insert(it.key(), vec);
    }
    _mux.unlock();

//    qDebug() << "removered" << client;

}

QVector<IClient *> Subscriber::get_subscribers(int type)
{
    return _map_subscribe.value(type);
}


/// signal data ready in some type
void Subscriber::data_ready(int type, QSharedPointer<IMessageOld> msg_ptr)
{
    emit signalGetData(type, msg_ptr);
//    double d = *(double*)(*(msg_ptr.data()).data());//*(double*)(data);
//    QSharedPointer<IMessage> msg_ptr(msg);
}

void Subscriber::_slotGetData(int type, rpc_msg msg_ptr)
{
//    qDebug() << "Getting new data with type = " << type;
    int id = 0;
//    QDataStream ds(*dd);
//    double r;
//    ds >> r;
//    qDebug() << "Send data to = " << id;

//    _mux.lock();
    QVector<IClient *> cls = _map_subscribe.value(type);
//    qDebug() << "counts = " << cls.count();
    for(int i = 0; i < cls.count(); i++)
    {
        if(!cls.at(i))
            return;
        int cl_id = cls.at(i)->get_id();
        int cl_type = cls.at(i)->get_type();
        if((cl_type == 1) || (cl_id == id))
        {
            cls.at(i)->send_data(msg_ptr);
        }
    }
//    _mux.unlock();
}
