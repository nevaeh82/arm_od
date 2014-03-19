#ifndef BLACONTROLLER_H
#define BLACONTROLLER_H

#include <QObject>
#include <QString>
#include <QMap>

#include "../ITCPController.h"
#include "../ITCPClient.h"
#include "../../Common/IRouter.h"
#include "../../Common/Router.h"
#include "../TCPClient.h"
#include "BLAControllerParser.h"
#include "BLAParser.h"
#include "IBLAController.h"
#include "BLA.h"

class BLAController : public QObject, public IBLAController
{
    Q_OBJECT
public:
    BLAController(IRouter *router);
    ~BLAController();

private:
    ITCPController      *_tcp_controller;
    IRouter             *_router;
    QString             _ip;
    quint16             _port;
    const int           _id;
    const int           _dev_control_BLA;

    BLAControllerParser             *_parser;
    QMap<quint16, ITCPClientOld *>     _map_clients;

public:
    void setController(QString ip, quint16 port);
    virtual void connect_bla(QVector<quint16>);

private slots:
    void _send_request(bool state);
};

#endif // BLACONTROLLER_H
