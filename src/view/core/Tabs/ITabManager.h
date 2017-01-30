#ifndef ITABMANAGER_H
#define ITABMANAGER_H

#include <QString>

class IMessageOld;
class MapController;

class ITabManager
{
public:
	~ITabManager(){}

    virtual void send_data(int pid, IMessageOld *data)     = 0;
    virtual void send_data_niipp_control(int id, QByteArray ba) = 0;

	virtual void set_arm_r_connection(bool) = 0;
};

#endif // ITABMANAGER_H
