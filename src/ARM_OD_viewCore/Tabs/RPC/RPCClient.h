#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>

#include <QTextCodec>
#include <QSettings>
#include <QStringList>

#include "IRPC.h"
#include "../../Common/IMessage.h"
#include "../../Common/CommandMessage.h"

#include "../TabsProperty.h"
#include "../Tree/DBManager.h"
#include "../ITabMap.h"
#include "../../MapInterface/IMapController.h"

#include "RadiolocationInterface.h"

#include "Rpc/RpcDefines.h"

class RPCClient : public QObject, public IRPC
{
    Q_OBJECT
public:
    RPCClient(TabsProperty *prop, IDBManager *db_manager, IDBManager *db_manager_target, IMapController* map_controller, ITabMap *parent_tab, ITabManager* tab_manager);
    ~RPCClient();


private slots:
    virtual int start();
    virtual int stop();

public:
	void set_command(IMessageOld* msg);

private slots:
	void _slotSetCommand(IMessageOld* msg);

private:
    QxtRPCPeer*         _rpc_client;
    QString             _ip_RPC;
    quint16             _port_RPC;
	IMessageOld*           _command_msg;
    IDBManager*         _db_manager;
    IDBManager*         _db_manager_target;
    TabsProperty*       _tab_property;
    ITabMap*            _parent_tab;
    ITabManager*        _tab_manager;
    IMapController*     _map_controller;

    QMap<int, int>      _map_peleng_evil_ids;
    int                 _peleng_evil_ids;
    int                 _rds_evil_ids;

    bool                _solver_auto;

private:
//    int     _init();

    int     _read_settings(QString path_to_ini_file_RPC);
	void    _form_command(IMessageOld *msg);
    void    _set_solver_auto(QByteArray ba);
//    void    _recognize();
//    void    _ss_correlation(bool enable);

//    void    _prm_set_freq(short freq);
//    void    _prm_request_freq();
//    void    _prm_set_att1(int att1);
//    void    _prm_set_att2(int att2);
//    void    _prm_set_filter(int index);

private slots:
    void _close();
    void _slotRCPConnetion();
    void _slotRPCDisconnection();
    void _slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
    void _slotReconnection();

signals:
	void signalSetCommand(IMessageOld *msg);
    void signalFinished();

    void signalStart();
    void signalStop();
    void signalFinishRPC();

    ///RPC signals
signals:
    void signalSetClientId(int id);
    void signalSetNIIPPBPLA(QByteArray data);
    void signalSetSolverData(QByteArray data);
    void signalSetSolverDataClear(QByteArray data);
//    void signalSetBandwidth(int id, float db);
//    void signalSetShift(int id, float shift);
//    void signalRecognize(int id, int type);
//    void signalSSCorrelation(int id, bool enable);

//    void signalPRMSetFreq(int id, short freq);
//    void signalPRMRequestFreq(int id);
//    void signalPRMSetAtt1(int id, int att1);
//    void signalPRMSetAtt2(int id, int att2);
//    void signalPRMSetFilter(int id, int index);


    void signalReconnection();

//    void signalDataS(float*,float*);
//    void signalData(float*,float*);

public slots:
    void slotInit();
    void slotStart();
    void slotStop();
    void slotFinish();

    ///rpc_server
    void rpc_send_bla_points(int id, QPointF point, double alt, double speed, double course, int state);
    void rpc_slot_server_send_ais_data(QByteArray data);
    void rpc_send_bpla_points(QByteArray data);
    void rpc_send_bpla_points_auto(QByteArray data);
    void rpc_send_niipp_data(QByteArray data);

    void rpc_slot_server_send_atlant_direction(QByteArray data);
    void rpc_slot_server_send_atlant_position(QByteArray data);


private:
    void _send_niipp_bpla(QByteArray data);
    void _send_data_to_sovler(QByteArray data);
    void _set_solver_clear(QByteArray data);

    void _send_bpla_points(QByteArray data);
//    void _rpc_slot_set_niipp_data(QByteArray data);
//    void _send_niipp_mode(int value);
//    void rpc_slot_getting_points(rpc_send_points_vector points);
//    void rpc_slot_getting_modulation(QString modulation);
//    void rpc_slot_server_send_correlation(int point1, int point2, rpc_send_points_vector points);

//    void rpc_slot_server_prm_status(int prm_freq, int prm_filter, int prm_att1, int prm_att2);

};

#endif // RPCCLIENT_H
