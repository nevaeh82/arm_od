#ifndef ROUTER_H
#define ROUTER_H

#include <QObject>
#include <QThread>

#include "IRouter.h"
#include "IQueue.h"
#include "ISubscriber.h"
#include "TCP/ITCPController.h"

class Router : public QObject, public IRouter
{
    Q_OBJECT
public:
    Router();
    ~Router();

public:
    virtual void set_queue(IQueue *queue);
    virtual void set_tcp_controller(ITCPController *tcp_controller);
    virtual IQueue                  *get_queue();
    virtual ITCPController          *get_tcp_controller();

    virtual void set_subscriber(ISubscriber *scr);
    virtual ISubscriber* get_subscriber();


private:
    IQueue                  *_queue;
    ITCPController          *_tcp_controller;
    ISubscriber*            _subscriber;
};

#endif // ROUTER_H
