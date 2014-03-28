QT += core network
TARGET   = ARM_OD_ServerCore
TEMPLATE = lib
ProjectDir = $$PWD

# or EXPORT class need define.
CONFIG += staticlib

include(../../specs/project.pri)
include(../ARM_OD_Common/reference.pri)
include(../../specs/qxt.pri)
include(../../specs/Solver.pri)
include(../../specs/protobuf.pri)
include($$SolutionDir/specs/CISCommonLib.pri )
include(../../specs/logger.pri)

INCLUDEPATH += $$PWD

HEADERS += \
    ARM_OD_Application.h \
    ARM_OD_Srv.h \
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
    RPC/RPCServer.h \
    TCP/TcpDevicesDefines.h \
    TCP/TcpDefines.h \
    TCP/TcpNIIPPCoder.h \
    TCP/TcpNIIPPController.h \
    TCP/TcpKTRCoder.h \
    TCP/TcpKTRController.h \
    TCP/TcpManager.h \
    TCP/TcpAISClient.h \
    TCP/TcpAISCoder.h \
    TCP/TcpManager.h \
    TCP/UAV/IBLAController.h \
    TCP/TcpArmrClientController.h \
    TCP/TcpArmrClientCoder.h \
    TCP/TcpAISController.h \
    TCP/TcpAISZoneManager.h \
    TCP/TcpAISZoneDefines.h \
    TCP/Interfaces/ITcpAISZoneManager.h \
    TCP/Interfaces/ITcpSettingsManager.h \
    TCP/TcpSettingsManager.h

SOURCES += \
    ARM_OD_Application.cpp \
    ARM_OD_Srv.cpp \
    Common/Simulators/AtlantSimulator.cpp \
    Common/CRCs.cpp \
    Common/Router.cpp \
    Common/Subscriber.cpp \
    RPC/Client/RPCClient_R.cpp \
    RPC/Message.cpp \
    RPC/RPCClient.cpp \
    RPC/RPCServer.cpp \
    TCP/TcpNIIPPCoder.cpp \
    TCP/TcpNIIPPController.cpp \
    TCP/TcpKTRCoder.cpp \
    TCP/TcpKTRController.cpp \
    TCP/TcpManager.cpp \
    TCP/TcpAISClient.cpp \
    TCP/TcpAISCoder.cpp \
    TCP/TcpAISController.cpp \
    TCP/UAV/BLASimulator.cpp \
    TCP/TcpArmrClientController.cpp \
    TCP/TcpArmrClientCoder.cpp \
    TCP/TcpAISZoneManager.cpp \
    TCP/TcpSettingsManager.cpp
