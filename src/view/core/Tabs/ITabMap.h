#ifndef ITABMAP_H
#define ITABMAP_H

#include <QVector>
#include <QMap>
#include <QVariant>

#include "../Common/IMessage.h"

class ITabMap
{
public:
    virtual ~ITabMap(){};

//    virtual void set_selected_area(QMap<int, QVariant> data)        = 0; //1 - selection, 2 - central freq, 3 - begin freq, 4 - end freq

//    virtual void set_command(IMessage *msg)                         = 0;

//    virtual void set_points_rpc(QVector<QPointF> points)            = 0;
};

#endif // ITABMAP_H
