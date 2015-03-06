#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "../../Tabs/MainTreeModelManager.h"

class IController
{
public:
    ~IController(){};
    virtual void setupData()                                                = 0;
//    MainTabView *get_view();
    virtual MainTreeModelManager    *get_model()                            = 0;
    virtual void reset_model(int id, int id_view)                           = 0;
    virtual void update_property(int pid, int id,/*QString name,*/ QVariant value)     = 0;
};

#endif // ICONTROLLER_H
