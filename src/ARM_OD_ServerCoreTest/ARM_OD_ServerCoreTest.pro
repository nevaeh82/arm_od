TARGET   = ARM_OD_ServerCoreTest
TEMPLATE = app

ProjectDir=$$PWD

QT     +=  core network
QT     -= gui

CONFIG += console
CONFIG += qtestlib

INCLUDEPATH += ../../tools/cxxtest

HEADERS += test.h

include(../../specs/project.pri)
include(../ARM_OD_ServerCore/reference.pri)
include(../../specs/qxt.pri)
include(../../specs/protobuf.pri)
include(../TestShared/mksuite.pri)

applyReferences()
