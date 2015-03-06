#ifndef ARM_OD_APPLICATION_H
#define ARM_OD_APPLICATION_H

#include <qtsinglecoreapplication.h>
#include <stdexcept>

class ARM_OD_Application : public QtSingleCoreApplication
{
public:
    ARM_OD_Application(int& argc, char** argv);

    bool notify(QObject* receiver, QEvent* event);

};

#endif // ARM_OD_APPLICATION_H
