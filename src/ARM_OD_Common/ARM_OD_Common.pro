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
include($$SolutionDir/specs/CISCommonLib.pri )

INCLUDEPATH += $$PWD

SOURCES += \
    Rpc/RpcDefines.cpp \
    SettingsManager/DbBlaSettingsManager.cpp \
	SettingsManager/EnviromentSettingsManager.cpp

HEADERS += \
    Rpc/RpcDefines.h \
    Info/BaseSettings.h \
    Info/NIIPPSettings.h \
    Info/KTRSettings.h \
    Info/AISSettings.h \
    Info/ArmrTcpClientSettings.h \
    UAVDefines.h \
    Info/ArmrTcpClientSettings.h \
    SettingsManager/DbBlaSettingsManager.h \
    SettingsManager/EnviromentSettingsManager.h \
    Interfaces/SettingsManager/IDbBlaSettingsManager.h \
	Interfaces/SettingsManager/IEnviromentSettingsManager.h
