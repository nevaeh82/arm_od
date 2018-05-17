#-------------------------------------------------
#
# Project created by QtCreator 2013-10-25T13:43:51
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoReper
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	ReperWindow.cpp

FORMS += \
	ReperWindow.ui

HEADERS += \
	ReperWindow.h
