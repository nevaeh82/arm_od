QT += core gui network sql
CONFIG   += gui
CONFIG += console

TARGET   = ARM_OD_view
TEMPLATE = app
ProjectDir=$$PWD

SOURCES = main.cpp

include(../../specs/project.pri)
include(../ARM_OD_viewCore/reference.pri)
include(../ARM_OD_Common/reference.pri)
include(../../specs/CISCommonLib.pri )
include(../../specs/qxt.pri)
include(../../specs/logger.pri)
include (../../specs/pwgiswidget.pri )

#RESOURCES += \
#        rc.qrc

DEFINES += UNICODE
DEFINES += _UNICODE

applyReferences()
