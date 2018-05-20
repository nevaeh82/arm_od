#-------------------------------------------------
#
# Project created by QtCreator 2012-07-09T20:55:38
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoTacticMap
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	TacticMapWindow.cpp

FORMS += \
	TacticMapWindow.ui

HEADERS += \
	TacticMapWindow.h



include( ../../specs/pwgiswidget.pri )

