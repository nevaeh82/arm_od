#-------------------------------------------------
#
# Project created by QtCreator 2014-01-10T15:15:51
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoSatelliteSpot
CONFIG   -= app_bundle
TEMPLATE = app
ProjectDir = $$PWD

include( ../../specs/pwgiswidget.pri )
#include( ../../src/GeoMathematics/reference.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	SatelliteSpotWindow.cpp \
	SatelliteMapWidget.cpp \
	SpaceModel.cpp \
	SatelliteSpot.cpp

FORMS += \
	SatelliteSpotWindow.ui \
	SatelliteMapWidget.ui

HEADERS += \
	SatelliteSpotWindow.h \
	SatelliteMapWidget.h \
	SpaceModel.h \
	SatelliteSpot.h \
	SatelliteSpot.h

RESOURCES += \
	DemoSatelliteSpot.qrc













