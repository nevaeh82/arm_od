#ifndef ISUBMODULE_H
#define ISUBMODULE_H

class QWidget;
class QString;

class ISubModule
{
public:
	virtual ~ISubModule(){}
    virtual int start()                             = 0;
    virtual int stop()                              = 0;
    virtual void show()                             = 0;
    virtual void hide()                             = 0;
    virtual int createRPC()                         = 0;
    virtual int closeRPC()                          = 0;
    virtual int createTree()                        = 0;
};

#endif // ISUBMODULE_H
