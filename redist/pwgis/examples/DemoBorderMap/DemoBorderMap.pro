#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T12:05:38
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoBorderMap
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	BorderMapWindow.cpp

FORMS += \
	BorderMapWindow.ui

HEADERS += \
	BorderMapWindow.h
