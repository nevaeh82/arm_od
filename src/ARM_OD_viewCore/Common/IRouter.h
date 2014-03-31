#ifndef IROUTER_H
#define IROUTER_H

//enum CommandTypeEnum
//{
//    CommandToCom        = 0,
//    CommandToFPGA       = 1,
//    CommandToTelemetry  = 2,
//    CommandToPhoto      = 3
//}CommandType;

class ITreeModel;
//class ITCPController;


class IRouter
{
public:
    virtual ~IRouter(){};
//    virtual void set_tcp_controller(ITCPController *tcp_controller)     = 0;
//    virtual ITCPController          *get_tcp_controller()               = 0;
    virtual ITreeModel             *get_models_controller(int id_tree)            = 0;
};

#endif // IROUTER_H
