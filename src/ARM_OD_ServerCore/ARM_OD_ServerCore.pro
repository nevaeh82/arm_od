QT += core network
TARGET   = ARM_OD_ServerCore
TEMPLATE = lib
ProjectDir = $$PWD

# or EXPORT class need define.
CONFIG += staticlib

include(../../specs/project.pri)
include(../ARM_OD_Common/reference.pri)
include(../../specs/protobuf.pri)
include($$SolutionDir/specs/CISCommonLib.pri )
include(../../specs/qxt.pri)
include(../../specs/Solver.pri)
include(../../specs/logger.pri)

INCLUDEPATH += $$PWD

HEADERS += \
    ARM_OD_Application.h \
    ARM_OD_Srv.h \
    AIS/Ais.h \
    AIS/parser.h \
    Common/Simulators/AtlantSimulator.h \
    Common/CRCs.h \
    Common/IClient.h \
    Common/IMessage.h \
    Common/IQueue.h \
    Common/IRouter.h \
    Common/ISubscriber.h \
    Common/Router.h \
    Common/Subscriber.h \
    RPC/Client/IRPC_R.h \
    RPC/Client/RPCClient_R.h \
    RPC/IRPC.h \
    RPC/Message.h \
    RPC/RPCClient.h \
#    RPC/RPCClientFlakon.h \
    RPC/RPCServer.h \
    TCP/ITCPClient.h \
    TCP/ITCPController.h \
    TCP/ITCPParser.h \
#    TCP/ParserKTR.h \
#    TCP/PrmClient.h \
    TCP/TCPClient.h \
    TCP/TCPController.h \
    TCP/TCPPacketStruct.h \
    TCP/NIIPP/INIIPPController.h \
    TCP/NIIPP/NIIPPController.h \
    TCP/NIIPP/NIIPPParser.h \
    TCP/UAV/BLA.h \
    TCP/UAV/BLAController.h \
    TCP/UAV/BLAControllerParser.h \
    TCP/UAV/BLAParser.h \
    TCP/UAV/BLAParser2.h \
    TCP/UAV/BLASimulator.h \
    TCP/UAV/IBLAController.h \
    TCP/TcpDevicesDefines.h \
    TCP/TcpDefines.h \
    TCP/TcpNIIPPCoder.h \
    TCP/TcpNIIPPController.h \
    TCP/TcpKTRCoder.h \
    TCP/TcpKTRController.h \
    TCP/Interfaces/ITcpManager.h \
    TCP/TcpManager.h \
    TCP/TcpAISClient.h \
    TCP/TcpAISCoder.h \
    TCP/TcpAISController.h \
    TCP/TcpAISZoneManager.h

SOURCES += \
    ARM_OD_Application.cpp \
    ARM_OD_Srv.cpp \
    AIS/Ais.cpp \
    AIS/parser.cpp \
    Common/Simulators/AtlantSimulator.cpp \
    Common/CRCs.cpp \
    Common/Router.cpp \
    Common/Subscriber.cpp \
    RPC/Client/RPCClient_R.cpp \
    RPC/Message.cpp \
    RPC/RPCClient.cpp \
#    RPC/RPCClientFlakon.cpp \
    RPC/RPCServer.cpp \
#    TCP/ParserKTR.cpp \
#    TCP/PrmClient.cpp \
#    TCP/source.cpp \
    TCP/TCPClient.cpp \
    TCP/TCPController.cpp \
    TCP/NIIPP/NIIPPController.cpp \
    TCP/NIIPP/NIIPPParser.cpp \
    TCP/UAV/BLA.cpp \
    TCP/UAV/BLAController.cpp \
    TCP/UAV/BLAControllerParser.cpp \
    TCP/UAV/BLAParser.cpp \
    TCP/UAV/BLAParser2.cpp \
    TCP/UAV/BLASimulator.cpp \
    TCP/TcpNIIPPCoder.cpp \
    TCP/TcpNIIPPController.cpp \
    TCP/TcpKTRCoder.cpp \
    TCP/TcpKTRController.cpp \
    TCP/TcpManager.cpp \
    TCP/TcpAISClient.cpp \
    TCP/TcpAISCoder.cpp \
    TCP/TcpAISController.cpp \
    TCP/TcpAISZoneManager.cpp
