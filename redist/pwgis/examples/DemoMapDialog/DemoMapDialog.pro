#-------------------------------------------------
#
# Project created by QtCreator 2013-06-13T11:05:51
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoMapDialog
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	MapDialogWindow.cpp

FORMS += \
	MapDialogWindow.ui

HEADERS += \
	MapDialogWindow.h
