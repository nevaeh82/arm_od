#ifndef TCPDEFINES_H
#define TCPDEFINES_H

#define TCP_ZAVIRUHA_PREAMBULA "ZAV"
#define TCP_ZAVIRUHA_PREAMBULA_LEN 7

/// NIIPP device internal
#define TCP_NIIPP_ANSWER		"tcpNiippAnswer"
#define TCP_NIIPP_REQUEST_PBLA	"tcpNiippRequestBpla"

/// KTR device internal
#define TCP_KTR_ANSWER_BOARD_LIST			"tcpKtrAnswerBoardList"
#define TCP_KTR_REQUEST_GET_BOARD_LIST		"tcpKtrRequestGetBoardList"
#define TCP_KTR_ANSWER_BPLA					"tcpKtrAnswerBpla"
#define TCP_KTR_REQUEST_COMMAND_TO_BPLA		"tcpKtrRequestCommandToBpla"

/// ARMR client internal
#define TCP_ARMR_SEND_ATLANT_DIRECTION		"tcpArmrSendAtlantDirection"
#define TCP_ARMR_SEND_ATLANT_POSITION		"tcpArmrSendAtlantPosition"
#define TCP_ARMR_SEND_BPLA_POINTS_AUTO		"tcpArmrSendBplaPointsAuto"
#define TCP_ARMR_SEND_BPLA_POINTS			"tcpArmrSendBplaPoints"

#define TCP_ARMR_SEND_SOLVER_DATA		"tcpArmrSendSolverData"
#define TCP_ARMR_SEND_SOLVER_CLEAR		"tcpArmrSendSolverClear"

/// AIS device internal
#define TCP_AIS_ANSWER_DATA	"tcpAisAnswerData"

#define TCP_AIS_URL_STANDART		"http://bma.fr24.com/zones/"
#define TCP_AIS_URL_FULL_MAP		"full_all.js"
#define TCP_AIS_URL_EUROPE			"europe_all.js"
#define TCP_AIS_URL_NORTHAMERICA	"northamerica_all.js"
#define TCP_AIS_URL_SOUTHAMERICA	"southamerica_all.js"
#define TCP_AIS_URL_OCEANIA			"oceania_all.js"
#define TCP_AIS_URL_ASIA			"asia_all.js"
#define TCP_AIS_URL_AFRICA			"africa_all.js"
#define TCP_AIS_URL_ATLANTIC		"atlantic_all.js"
#define TCP_AIS_URL_MALDIVES		"maldives_all.js"
#define TCP_AIS_URL_NORTHATLANTIC	"northatlantic_all.js"

#endif // TCPDEFINES_H
