#ifndef ATLANTSIMULATOR_H
#define ATLANTSIMULATOR_H

#include <QObject>
#include <QPointF>
#include <QDataStream>
#include <QByteArray>
#include <math.h>
#include <QDateTime>

#include "../../Common/Router.h"
#include "../../Common/ISubscriber.h"
#include "../../RPC/Message.h"
#include "../../RPC/Client/RPCClient_R.h"

class AtlantSimulator : public QObject, public IClient
{
        Q_OBJECT
    public:
        AtlantSimulator(IRouter* router, double lat, double lon, int sourceid);
        ~AtlantSimulator();

    public:
        virtual void set_id(int id);
        virtual int get_id();
        virtual void set_type(int type);
        virtual int get_type();
        virtual void send_data(QSharedPointer<IMessageOld> msg_ptr);

    private slots:
        void _slot_simulation(QSharedPointer<IMessageOld> msg_ptr);

    private:
        int             _id;
        int             _type;
        IRouter*        _router;
        ISubscriber*    _subscriber;

        double _lat;
        double _lon;
        int _sourceid;

signals:
        void signalData(QSharedPointer<IMessageOld>);

        void signalFinished();
};

#endif // ATLANTSIMULATOR_H
