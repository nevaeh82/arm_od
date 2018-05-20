#-------------------------------------------------
#
# Project created by QtCreator 2013-03-21T15:03:21
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoPostTracking
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	PostTrackingWindow.cpp

FORMS += \
	PostTrackingWindow.ui

HEADERS += \
	PostTrackingWindow.h
