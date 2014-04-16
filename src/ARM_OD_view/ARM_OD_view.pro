QT += core gui network sql
CONFIG   += gui

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

RESOURCES += \
	../ARM_OD_viewCore/rc.qrc \
	../ARM_OD_viewCore/translations.qrc

win32{
    HEADERS = version.h
    RC_FILE = details_res.rc
}

