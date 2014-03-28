#ifndef RPCCLIENT_R_H
#define RPCCLIENT_R_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>
#include <QDataStream>

#include <QTextCodec>
#include <QSettings>
#include <QStringList>

#include "IRPC_R.h"
#include "../../Common/IMessage.h"
//#include "../../Common/CommandMessage.h"
#include "../../Common/IRouter.h"
#include "../../Common/ISubscriber.h"
#include "../../RPC/Message.h"

#include "../../Common/IClient.h"

#include "Rpc/RpcDefines.h"

class RPCClient_R : public QObject, public IClient
{
    Q_OBJECT
public:
    RPCClient_R(IRouter* router);
    ~RPCClient_R();

public:
    virtual void set_id(int id);
    virtual int get_id();
    virtual void set_type(int type);
    virtual int get_type();
	virtual void send_data(QSharedPointer<IMessageOld> msg_ptr);


private slots:
    virtual int start();
    virtual int stop();

public:
	void set_command(IMessageOld* msg);
    void push_msg(QByteArray msg);

private slots:
	void _slotSetCommand(IMessageOld* msg);
    void _slotPushMsg(QByteArray msg);
	void _slotGetData(QSharedPointer<IMessageOld> msg_ptr);


private:
    QxtRPCPeer*         _rpc_client;
    QString             _ip_RPC;
    quint16             _port_RPC;
	IMessageOld*           _command_msg;
    int                 _id;
    IRouter*            _router;
    ISubscriber*        _subscriber;

    int                 _type;

private:
//    int     _init();

    int     _read_settings(QString path_to_ini_file_RPC);
	void    _form_command(IMessageOld *msg);

private slots:
    void _close();
    void _slotRCPConnetion();
    void _slotRPCDisconnection();
    void _slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
    void _slotReconnection();

signals:
	void signalSetCommand(IMessageOld *msg);
    void signalPushMsh(QByteArray* data);
    void signalFinished();

	void signalSendData(QSharedPointer<IMessageOld> msg_ptr);

    void signalStart();
    void signalStop();
    void signalFinishRPC();

    ///RPC signals
signals:
    void signalSetClientId(int id);
    void signalSetSolver(QByteArray data);
    void signalSetSolverClear(QByteArray data);


    void signalReconnection();

public slots:
    void slotInit();
    void slotStart();
    void slotStop();
    void slotFinish();

    ///rpc_server
    void rpc_slot_server_send_bpla_def(QByteArray ba);
    void rpc_slot_server_send_bpla_def_auto(QByteArray ba);

    void rpc_slot_server_atlant_direction(QByteArray ba);
    void rpc_slot_server_atlant_position(QByteArray ba);


//    void rpc_slot_server_send_bpla_def(QByteArray ba);

//    void _rpc_slot_set_niipp_data(QByteArray data);


};

#endif // RPCCLIENT_R_H
