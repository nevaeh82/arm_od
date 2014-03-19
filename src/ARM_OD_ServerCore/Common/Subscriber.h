#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QMap>
#include <QVector>

#include <QDataStream>
#include <QMutex>

#include <QSharedPointer>
#include <QMetaType>
#include "IClient.h"
#include "IMessage.h"


#include "ISubscriber.h"

typedef QSharedPointer<IMessageOld> rpc_msg;

class Subscriber : public QObject, public ISubscriber
{
    Q_OBJECT
public:
    Subscriber();
    ~Subscriber();

public:
    virtual void add_subscription(int type, IClient* client);
    virtual void remove_subscription(int type, IClient* client);
    virtual void remove_subscription(int type);
    virtual void remove_subscription(IClient* client);

    QVector<IClient *> get_subscribers(int type);

    virtual void data_ready(int type, QSharedPointer<IMessageOld> msg);


private:
    QMap<int, QVector<IClient *> >  _map_subscribe;

    QMutex          _mux;

signals:
    void signalGetData(int type, rpc_msg msg_ptr);

private slots:
    void _slotGetData(int type, rpc_msg msg_ptr);
};



#endif // SUBSCRIBER_H
