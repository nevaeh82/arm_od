QT       += core network
QT       -= gui

TARGET = PrmSimulator

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

ProjectDir = $$PWD

SOURCES += main.cpp \
	PrmSimulator.cpp

HEADERS += \
	PrmSimulator.h

include( ../../specs/project.pri )
include( ../../specs/CISCommonLib.pri )
include( ../../specs/qxt.pri )
include( ../ARM_OD_Common/reference.pri )
