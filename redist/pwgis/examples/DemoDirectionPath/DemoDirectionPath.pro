#-------------------------------------------------
#
# Project created by QtCreator 2013-03-14T14:28:44
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoDirectionPath
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	DemoDirectionPath.cpp

FORMS += \
	DemoDirectionPath.ui

HEADERS += \
	DemoDirectionPath.h
