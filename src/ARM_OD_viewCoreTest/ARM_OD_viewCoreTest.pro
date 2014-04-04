TARGET   = ARM_OD_viewCoreTest
TEMPLATE = app

QT     +=  core sql network
QT     += gui
CONFIG += console
CONFIG += qtestlib

ProjectDir=$$PWD

INCLUDEPATH += ../../tools/cxxtest

HEADERS += \
    testdb.h

include(../../specs/project.pri)
include(../ARM_OD_viewCore/reference.pri)
include(../ARM_OD_Common/reference.pri)
include($$SolutionDir/specs/CISCommonLib.pri )
include(../../specs/qxt.pri)
include(../../specs/logger.pri )
include(../TestShared/mksuite.pri)

applyReferences()

#LIBS += -lsetupapi -luuid -ladvapi32

#LIBS +=  -L../../build/bin/msvc -lARM_OD_viewCored
#LIBS += -luser32
