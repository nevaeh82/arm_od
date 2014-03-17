QT += core network sql
QT -= gui

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += qxt
QXT		 += core network

TARGET   = ARM_OD_Server
TEMPLATE = app   
ProjectDir=$$PWD

SOURCES = main.cpp

include(../../specs/project.pri)
include(../ARM_OD_ServerCore/reference.pri)
include(../ARM_OD_Common/reference.pri)
include(../../specs/qxt.pri)
include(../../specs/Solver.pri)
include(../../specs/protobuf.pri)
include(../../specs/logger.pri)

applyReferences()
