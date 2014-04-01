#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "MainTreeModelManager.h"

class IController
{
public:
    ~IController(){};
    virtual void setupData()                        = 0;
//    MainTabView *get_view();
    virtual MainTreeModelManager    *get_model()    = 0;
    virtual void reset_model(int id)                = 0;
};

#endif // ICONTROLLER_H
