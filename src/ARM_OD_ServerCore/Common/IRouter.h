#ifndef IROUTER_H
#define IROUTER_H

class IQueue;
class IController;
class ISubscriber;

class IRouter
{
public:
    virtual ~IRouter(){};
	virtual void set_queue(IQueue *queue)                               = 0;
	virtual IQueue *get_queue()                                         = 0;
    virtual void set_subscriber(ISubscriber *scr)                       = 0;
    virtual ISubscriber* get_subscriber()                               = 0;
};

#endif // IROUTER_H
