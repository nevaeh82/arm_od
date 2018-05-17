#-------------------------------------------------
#
# Project created by QtCreator 2013-09-30T13:15:51
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoAtlas
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	AtlasWindow.cpp

FORMS += \
	AtlasWindow.ui

HEADERS += \
	AtlasWindow.h
