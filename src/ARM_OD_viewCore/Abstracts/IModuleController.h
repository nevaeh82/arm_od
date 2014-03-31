#ifndef IMODULECONTROLLER_H
#define IMODULECONTROLLER_H

class QString;

class IModuleController
{
public:
	virtual ~IModuleController(){}
    virtual int start()                                         = 0;
    virtual int stop()                                          = 0;
    virtual void show()                                         = 0;
    virtual void hide()                                         = 0;
    virtual int createSubModules(QString path_to_ini_file)      = 0;
};

#endif // IMODULECONTROLLER_H
