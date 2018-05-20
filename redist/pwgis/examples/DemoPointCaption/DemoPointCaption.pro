#-------------------------------------------------
#
# Project created by QtCreator 2013-04-29T15:11:24
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoPointCaption
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	PointCaptionWindow.cpp

FORMS += \
	PointCaptionWindow.ui

HEADERS += \
	PointCaptionWindow.h
