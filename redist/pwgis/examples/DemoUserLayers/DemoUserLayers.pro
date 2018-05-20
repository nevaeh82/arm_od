#-------------------------------------------------
#
# Project created by QtCreator 2013-02-25T11:42:55
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoUserLayers
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include(../../specs/pwgiswidget.pri)

SOURCES += main.cpp \
	UserLayersWindow.cpp

HEADERS  += \
	UserLayersWindow.h

FORMS    += \
	UserLayersWindow.ui

RESOURCES += \
    DemoUserLayers.qrc
