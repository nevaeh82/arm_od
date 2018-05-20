#-------------------------------------------------
#
# Project created by QtCreator 2013-03-05T11:17:15
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoAtlasProfiles
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
    DemoAtlasProfiles.cpp

FORMS += \
    DemoAtlasProfiles.ui

HEADERS += \
    DemoAtlasProfiles.h

RESOURCES += \
    DemoAtlasProfiles.qrc
