#ifndef TCPDEVICESDEFINES_H
#define TCPDEVICESDEFINES_H

//#define NIIPP_TCP_DEVICE	"niippTcpDevice"
//#define KTR_TCP_DEVICE		"ktrTcpDevice"
//#define AIS_TCP_DEVICE		"aisTcpDevice"
#define ARMR_TCP_CLIENT_NAME "armrTcpClient"

namespace DeviceTypes {
typedef enum DeviceTypesEnum
{
	NIIPP_TCP_DEVICE = 1,
	KTR_TCP_DEVICE = 2,
	AIS_TCP_DEVICE = 3,
	ARMR_TCP_CLIENT = 4
} DeviceTypesEnum;
}



#endif // TCPDEVICESDEFINES_H
