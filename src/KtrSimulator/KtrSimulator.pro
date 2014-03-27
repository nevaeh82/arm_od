QT       += core network
QT       -= gui

TARGET = KtrSimulator

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

ProjectDir = $$PWD

SOURCES += main.cpp \
	KtrSimulator.cpp

HEADERS += \
	KtrSimulator.h

include( ../../specs/project.pri )
include( ../../specs/CISCommonLib.pri )
