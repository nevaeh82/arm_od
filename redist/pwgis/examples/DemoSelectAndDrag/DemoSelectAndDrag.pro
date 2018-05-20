#-------------------------------------------------
#
# Project created by QtCreator 2013-03-21T15:03:21
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoSelectAndDrag
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	SelectAndDragWindow.cpp

FORMS += \
	SelectAndDragWindow.ui

HEADERS += \
	SelectAndDragWindow.h

RESOURCES += \
	DemoSelectAndDrag.qrc

OTHER_FILES += \
	profile/init.js
