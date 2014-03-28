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

#include "Station.h"
#include "../Tree/DBManager.h"
#include "../ITabMap.h"
#include "../../MapInterface/IMapController.h"

#include "RadiolocationInterface.h"

#define RPC_SLOT_SET_CLIENT_ID   "rpc_slot_set_client_id"

/// to NIIPP
#define RPC_SLOT_SET_NIIPP_BPLA    "rpc_slot_set_niipp_data"

/// to SOLVER
#define RPC_SLOT_SET_SOLVER_DATA    "rpc_slot_set_solver_data"
#define RPC_SLOT_SET_SOLVER_CLEAR   "rpc_slot_set_solver_clear"


//#define RPC_SLOT_SET_BANDWIDTH   "rpc_slot_set_bandwidth"
//#define RPC_SLOT_SET_SHIFT       "rpc_slot_set_shift"
//#define RPC_SLOT_RECOGNIZE       "rpc_slot_recognize"
//#define RPC_SLOT_SS_CORRELATION  "rpc_slot_ss_correlation"


/////PRM300
//#define RPC_SLOT_PRM_SET_FREQ       "rpc_slot_prm_set_freq"
//#define RPC_SLOT_PRM_REQUEST_FREQ   "rpc_slot_prm_request_freq"
//#define RPC_SLOT_PRM_SET_ATT1       "rpc_slot_prm_set_att1"
//#define RPC_SLOT_PRM_SET_ATT2       "rpc_slot_prm_set_att2"
//#define RPC_SLOT_PRM_SET_FILTER     "rpc_slot_prm_set_filter"





///server
#define RPC_SLOT_SERVER_SEND_BLA_POINTS                 "rpc_slot_server_send_bla_points"

/// BPLA
#define RPC_SLOT_SERVER_SEND_BPLA_POINTS                "rpc_slot_server_send_bpla_point_from_arm_r"
#define RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO           "rpc_slot_server_send_bpla_point_from_arm_r_auto"

/// NIIPP
#define RPC_SLOT_SERVER_SEND_NIIPP_DATA                "rpc_slot_server_send_NIIPP_data"

/// AIS
#define RPC_SLOT_SERVER_SEND_AIS_DATA                 "rpc_slot_server_send_ais_data"



/// from ARM_R Atlant direction
#define RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION           "rpc_slot_server_send_atlant_direction"
#define RPC_SLOT_SERVER_SEND_ATLANT_POSITION            "rpc_slot_server_send_atlant_position"


//#define RPC_SLOT_SERVER_SEND_POINTS                 "rpc_slot_server_send_points"
//#define RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION    "rpc_slot_server_send_response_modulation"
//#define RPC_SLOT_SERVER_SEND_CORRELATION            "rpc_slot_server_send_correlation"

/////PRM from rpc server
//#define RPC_SLOT_SERVER_PRM_STATUS                   "rpc_slot_server_prm_status"



typedef QVector<QPointF>         rpc_send_points_vector;
typedef QPointF                  rpc_QPointF;
typedef QVector<DataFly>         rpc_data_fly;

typedef struct A_Dir_Ans_msg
{
    //id запроса
    int requestId;
    //id источника, при ответе на DirectionFindingRequest совпадает с айди запроса
    //              при ответе на ScanRequest назначается пеленгатором
    int sourceId;
    //время засечки
    //DateTime.Utc.Now - время в 100наносекундных интервалах начиная с 1 января 1 года
    quint64 dateTime;

    //имя поста
    QString post;
    double postLatitude;     //GPS Geo Coords
    double postLongitude;    //GPS Geo Coords
    double postHeight;       //meters

    //центральная частота
    double frequency;
    //ширина пика
    double widht;

    //пеленг
    double direction;
    //угол места
    double angle;
    //уровень
    double level;
    //качество
    double quality;

    int motionType;
    double motionConfidence;
}A_Dir_Ans_msg;

// POSITION_ANSWER_MESSAGE
typedef struct A_Pos_Ans_msg
{
    //id запроса
    int requestId;
    //id источника, при ответе на DirectionFindingRequest совпадает с айди запроса
    //              при ответе на ScanRequest назначается пеленгатором
    int sourceId;
    //время засечки
    quint64 dateTime;

     //Geo Coords
    double longitude;
     //Geo Coords
    double latitude;
     //качество
    double quality;
}A_Pos_Ans_msg;



class RPCClient : public QObject, public IRPC
{
    Q_OBJECT
public:
	RPCClient(Station* station, IDBManager *db_manager, IDBManager *db_manager_target, IMapController* map_controller, ITabMap *parent_tab, ITabManager* tab_manager);
	virtual ~RPCClient();


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
	Station*				m_station;
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
