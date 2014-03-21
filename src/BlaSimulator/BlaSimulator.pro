QT       += core network
QT       -= gui

TARGET = BlaSimulator

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

ProjectDir = $$PWD

SOURCES += main.cpp \
	BlaSimulator.cpp

include( ../../specs/project.pri )
include( ../../specs/CISCommonLib.pri )

HEADERS += \
	BlaSimulator.h
