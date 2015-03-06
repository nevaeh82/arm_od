#ifndef IROUTER_H
#define IROUTER_H

//enum CommandTypeEnum
//{
//    CommandToCom        = 0,
//    CommandToFPGA       = 1,
//    CommandToTelemetry  = 2,
//    CommandToPhoto      = 3
//}CommandType;

class IQueue;
//class IState;
class ITCPController;
class IExchangeMainTreeItem;

class IRouter
{
public:
	virtual ~IRouter(){}
    virtual void set_queue(IQueue *queue)                               = 0;
//    virtual void set_state(IState *state)                               = 0;
    virtual void set_tcp_controller(ITCPController *tcp_controller)     = 0;
    virtual void set_tcp_exchanger(IExchangeMainTreeItem *exchanger)    = 0;
    virtual IQueue *get_queue()                                         = 0;
//    virtual IState *get_state()                                         = 0;
    virtual ITCPController          *get_tcp_controller()               = 0;
    virtual IExchangeMainTreeItem   *get_tcp_exchanger()                = 0;
};

#endif // IROUTER_H
