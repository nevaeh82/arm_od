#-------------------------------------------------
#
# Project created by QtCreator 2011-07-12T15:47:00
#
#-------------------------------------------------

QT       += gui sql widgets
CONFIG      -= app_bundle

TARGET = DemoBasicFeatures
TEMPLATE = app
ProjectDir = $$PWD

SOURCES += main.cpp\
	DemoBasicFeatures.cpp


HEADERS  += \
	DemoBasicFeatures.h \
	ShowLog.h


FORMS    += DemoBasicFeatures.ui



RESOURCES +=



include(../../specs/pwgiswidget.pri)
