#-------------------------------------------------
#
# Project created by QtCreator 2013-09-30T13:15:51
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoAtlasTest
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	DemoAtlasTest.cpp

FORMS += \
	DemoAtlasTest.ui

HEADERS += \
	DemoAtlasTest.h
