#-------------------------------------------------
#
# Project created by QtCreator 2013-03-05T11:17:15
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoIconsReview
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
    DemoIconsReview.cpp

FORMS += \
    DemoIconsReview.ui

HEADERS += \
    DemoIconsReview.h

