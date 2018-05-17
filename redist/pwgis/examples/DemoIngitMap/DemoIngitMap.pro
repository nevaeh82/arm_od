#-------------------------------------------------
#
# Project created by QtCreator 2013-06-21T12:45:15
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoIngitMap
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	IngitMapWindow.cpp

FORMS += \
	IngitMapWindow.ui

HEADERS += \
	IngitMapWindow.h
