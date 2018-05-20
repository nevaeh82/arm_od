#-------------------------------------------------
#
# Project created by QtCreator 2013-04-18T14:41:35
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoPwGisWidgetVersion
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	PwGisWidgetVersionWindow.cpp

FORMS += \
	PwGisWidgetVersionWindow.ui

HEADERS += \
	PwGisWidgetVersionWindow.h
