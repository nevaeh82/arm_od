#-------------------------------------------------
#
# Project created by QtCreator 2012-02-20T17:09:28
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoGoogleCache
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	GoogleCacheMapWindow.cpp

FORMS += \
	GoogleCacheMapWindow.ui

HEADERS += \
	GoogleCacheMapWindow.h
