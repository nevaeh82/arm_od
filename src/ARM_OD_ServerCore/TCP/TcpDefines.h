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

#endif // TCPDEFINES_H
