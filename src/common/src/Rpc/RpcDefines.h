#ifndef RPCDEFINES_H
#define RPCDEFINES_H

#define RPC_SLOT_SET_CLIENT_ID          "rpc_slot_set_client_id"
#define RPC_SLOT_SET_MAIN_STATION_COR   "rpc_slot_set_main_station_cor"
#define RPC_SLOT_SET_BANDWIDTH          "rpc_slot_set_bandwidth"
#define RPC_SLOT_SET_SHIFT              "rpc_slot_set_shift"
#define RPC_SLOT_RECOGNIZE              "rpc_slot_recognize"
#define RPC_SLOT_SS_CORRELATION         "rpc_slot_ss_correlation"
#define RPC_SLOT_AVARAGE_SPECTRUM       "rpc_slot_avarage_spectrum"

///PRM300 from rpc client
#define RPC_SLOT_PRM_SET_FREQ       "rpc_slot_prm_set_freq"
#define RPC_SLOT_PRM_REQUEST_FREQ   "rpc_slot_prm_request_freq"
#define RPC_SLOT_PRM_SET_ATT1       "rpc_slot_prm_set_att1"
#define RPC_SLOT_PRM_SET_ATT2       "rpc_slot_prm_set_att2"
#define RPC_SLOT_PRM_SET_FILTER     "rpc_slot_prm_set_filter"

///server
#define RPC_SLOT_SERVER_SEND_POINTS                 "rpc_slot_server_send_points"
#define RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH		"rpc_slot_server_send_detected_bandwidth"
#define RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION    "rpc_slot_server_send_response_modulation"
#define RPC_SLOT_SERVER_SEND_CORRELATION            "rpc_slot_server_send_correlation"
#define RPC_SLOT_SERVER_SEND_HYPERBOLA				"rpc_slot_server_send_hyperbola"

///PRM to RPC client
#define RPC_SLOT_SERVER_PRM_STATUS                  "rpc_slot_server_prm_status"

///ATLANT
#define RPC_SLOT_SERVER_ATLANT_DIRECTION            "rpc_slot_server_atlant_direction"
#define RPC_SLOT_SERVER_ATLANT_POSITION             "rpc_slot_server_atlant_position"

/// ATLANT from client
#define RPC_SLOT_SET_ATLANT_FREQUENCY               "rpc_slot_set_atlant_frequency"

/// BPLA
#define RPC_SLOT_SERVER_SEND_BPLA_DEF               "rpc_slot_server_send_bpla_def"
#define RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO          "rpc_slot_server_send_bpla_def_auto"
#define RPC_SLOT_SERVER_SEND_BPLA_DEF_SINGLE        "rpc_slot_server_send_bpla_def_single"

/// connectiont status
#define RPC_SLOT_SERVER_STATUS                      "rpc_slot_server_status"

#define RPC_SLOT_REQUEST_STATUS                     "rpc_slot_request_status"

/// to solver
#define RPC_SLOT_SET_DATA_TO_SOLVER                 "rpc_slot_set_data_to_solver"
#define RPC_SLOT_SET_CLEAR_TO_SOLVER                "rpc_slot_set_clear_to_solver"

#define RPC_SLOT_SET_CLIENT_ID   "rpc_slot_set_client_id"

///server
#define RPC_SLOT_SERVER_SEND_BLA_POINTS               "rpc_slot_server_send_bla_points"

/// AIS
#define RPC_SLOT_SERVER_SEND_AIS_DATA                 "rpc_slot_server_send_ais_data"

/// from ARM_R
#define RPC_SLOT_SERVER_SEND_BPLA_POINTS              "rpc_slot_server_send_bpla_point_from_arm_r"
#define RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO         "rpc_slot_server_send_bpla_point_from_arm_r_auto"
#define RPC_SLOT_SERVER_SEND_BPLA_POINTS_SINGLE       "rpc_slot_server_send_bpla_point_from_arm_r_single"
#define RPC_SLOT_SERVER_SEND_HYPERBOLA                "rpc_slot_server_send_hyperbola"

/// NIIPP
#define RPC_SLOT_SERVER_SEND_NIIPP_DATA                "rpc_slot_server_send_NIIPP_data"
#define RPC_SLOT_SERVER_SEND_NIIPP_CONNECTION_STATUS   "rpc_slot_server_send_NIIPP_connection_status"

/// to NIIPP
#define RPC_SLOT_SET_NIIPP_BPLA                         "rpc_slot_set_niipp_data"
#define RPC_SLOT_NIIPP_BPLA                             "rpc_slot_niipp_bpla"
#define RPC_SLOT_GET_NIIPP_CONNECTION_STATUS                         "rpc_slot_get_niipp_connection_status"
/// to SOLVER
#define RPC_SLOT_SET_SOLVER_DATA    "rpc_slot_set_solver_data"
#define RPC_SLOT_SET_SOLVER_CLEAR   "rpc_slot_set_solver_clear"

/// from ARM_R Atlant direction
#define RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION           "rpc_slot_server_send_atlant_direction"
#define RPC_SLOT_SERVER_SEND_ATLANT_POSITION            "rpc_slot_server_send_atlant_position"

#define RPC_METHOD_AIS_REQUEST_GET_DATA		"rpcMethodAisRequestGetData"

#define RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST			"rpcMethodConfigRequestGetStationList"
#define RPC_METHOD_CONFIG_ANSWER_STATION_LIST				"rpcMethodConfigAnswerStationList"
#define RPC_METHOD_CONFIG_REQUEST_GET_MAP_OBJECTS			"rpcMethodConfigRequestGetMapObjects"
#define RPC_METHOD_CONFIG_ANSWER_MAP_OBJECTS				"rpcMethodConfigAnswerMapObjects"
#define RPC_METHOD_CONFIG_REQUEST_GET_MAP_POINTS			"rpcMethodConfigRequestGetMapPoints"
#define RPC_METHOD_CONFIG_ANSWER_MAP_POINTS					"rpcMethodConfigAnswerMapPoints"
#define RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION		"rpcMethodConfigRequestGetDbConfiguration"
#define RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION			"rpcMethodConfigAnswerDbConfiguration"

///PRM to RPC client
//#define RPC_SLOT_SERVER_PRM_STATUS                   "rpc_slot_server_prm_status"
#include <QObject>

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

#endif // RPCDEFINES_H
