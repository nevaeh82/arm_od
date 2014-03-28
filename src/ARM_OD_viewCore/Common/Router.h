#ifndef ROUTER_H
#define ROUTER_H

#include <QObject>
#include <QThread>
#include <QMap>

//#include "../Interfaces/Core/IRouter.h"
//#include "../Interfaces/Core/IQueue.h"
////#include "../Interfaces/Core/IState.h"
//#include "../Interfaces/core/ITCPController.h"
//#include "../Interfaces/core/IController.h"

#include "IRouter.h"
#include "../Tabs/Tree/ITreeModel.h"


class Router : public QObject, public IRouter
{
    Q_OBJECT
public:
    Router();
    ~Router();

public:
//    virtual void set_tcp_controller(ITCPController *tcp_controller);
//    virtual ITCPController          *get_tcp_controller();
    virtual ITreeModel             *get_models_controller(int id_tree);



private:
//    ITCPController          *_tcp_controller;
//    IModelController             *_models_controller;
    QMap<int, ITreeModel*> _map_model_controllers;

};

#endif // ROUTER_H
