#ifndef ITABMANAGER_H
#define ITABMANAGER_H

#include <QString>

class IMessageOld;
class MapController;

class ITabManager
{
public:
    ~ITabManager(){};

    virtual QString getStationName(int id)              = 0;
    virtual void send_data(int pid, IMessageOld *data)     = 0;
    virtual MapController* get_map_controller()         = 0;
    virtual void send_data_niipp_control(int id, QByteArray ba) = 0;

};

#endif // ITABMANAGER_H
