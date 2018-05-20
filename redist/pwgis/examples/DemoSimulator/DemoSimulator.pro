#-------------------------------------------------
#
# Project created by QtCreator 2013-03-21T15:03:21
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoSimulator
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
    SimulatorWindow.cpp \
    LineStyleGroupBox.cpp

FORMS += \
    SimulatorWindow.ui \
    LineStyleGroupBox.ui

HEADERS += \
    SimulatorWindow.h \
    LineStyleGroupBox.h

win32-msvc2010 {
 DEFINES += _USE_MATH_DEFINES
}
