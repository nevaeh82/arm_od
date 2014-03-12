#-------------------------------------------------
#
# Project created by QtCreator 2013-09-06T11:37:23
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = ARM_OD_Common
TEMPLATE = lib
CONFIG += staticlib

include(  ../../specs/project.pri )
include(  ../../specs/qxt.pri )
include(  ../../specs/logger.pri )

INCLUDEPATH += $$PWD

SOURCES += \
    ServiceHandler/ServiceHandler.cpp \
    ServiceHandler/CommonCodes.cpp \
    Rpc/RpcClientBase.cpp \
    Rpc/RpcServerBase.cpp

HEADERS += \
    ServiceHandler/ServiceTerminateCause.h \
    ServiceHandler/IServiceHandler.h \
    ServiceHandler/ServiceHandler.h \
    ServiceHandler/ProcessState.h \
    ServiceHandler/CommonCodes.h \
    ServiceHandler/ServiceTerminateCauseEnumsToString.h \
    ServiceHandler/CommonCodesEnumsToString.h \
    ServiceHandler/ProcessStateEnumsToString.h \
    Rpc/RpcClientBase.h \
    Rpc/RpcServerBase.h \
    Interfaces/IController.h \
    Interfaces/ITerminateRpcController.h \
    Settings/BaseItem.h \
    Settings/Object.h \
    Settings/Property.h \
    Settings/SettingsNode.h \
    BaseSubject.h \
    Interfaces/IRpcControllerBase.h \
    Interfaces/IRpcListener.h \
    Rpc/RpcDefines.h  
