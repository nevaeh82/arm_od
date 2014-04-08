#include "Router.h"

Router::Router()
{
	_queue = 0;
	_subscriber = 0;
}

Router::~Router()
{
}

void Router::set_queue(IQueue *queue)
{
    _queue = queue;
    QThread *thread_queue = new QThread;

    connect(thread_queue, SIGNAL(started()), _queue, SLOT(exec()));
    connect(_queue, SIGNAL(finished()), thread_queue, SLOT(quit()));
    connect(_queue, SIGNAL(finished()), thread_queue, SLOT(deleteLater()));
    connect(thread_queue, SIGNAL(finished()), thread_queue, SLOT(deleteLater()));

    _queue->moveToThread(thread_queue);
    thread_queue->start();
}

IQueue *Router::get_queue()
{
    return _queue;
}

void Router::set_subscriber(ISubscriber *scr)
{
    _subscriber = scr;
}

ISubscriber* Router::get_subscriber()
{
    return _subscriber;
}
