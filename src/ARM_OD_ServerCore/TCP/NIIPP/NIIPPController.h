#ifndef NIIPPCONTROLLER_H
#define NIIPPCONTROLLER_H

#include <QObject>

#include "../../Common/IRouter.h"
#include "../../Common/IClient.h"
#include "../../Common/IMessage.h"
#include "../../Common/ISubscriber.h"
#include "../../TCP/ITCPController.h"
#include "../TCPClient.h"
#include "NIIPPParser.h"

typedef QSharedPointer<IMessageOld> rpc_msg;


class NIIPPController : public QObject, public IClient
{
    Q_OBJECT
public:
    NIIPPController(IRouter *router);
    ~NIIPPController();

public:
    void setController(QString ip, quint16 port);

public:
    virtual void set_id(int id);
    virtual int get_id();
    virtual void set_type(int type);
    virtual int get_type();
	virtual void send_data(QSharedPointer<IMessageOld> msg_ptr);


private:
    IRouter*        _router;
    ISubscriber*    _subscriber;
    QString             _ip;
    quint16             _port;
    int                 _id;
    int                 _type;

    NIIPPParser*        _parser;

private slots:
	void _slotGetData(QSharedPointer<IMessageOld> msg_ptr);

private slots:
    void _slot_encode(QByteArray* data);

signals:
    void signalSendToEncodeBpla(QByteArray* data);

private:
    QByteArray _encode(QStringList list);
};

#endif // NIIPPCONTROLLER_H
