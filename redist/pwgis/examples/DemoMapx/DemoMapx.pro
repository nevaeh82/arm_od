#-------------------------------------------------
#
# Project created by QtCreator 2013-04-11T17:22:54
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoMapx
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	MapxWindow.cpp

FORMS += \
	MapxWindow.ui

HEADERS += \
	MapxWindow.h
