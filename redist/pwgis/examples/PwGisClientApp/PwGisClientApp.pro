#-------------------------------------------------
#
# Project created by QtCreator 2011-07-12T15:47:00
#
#-------------------------------------------------

QT       += webkit gui 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG      -= app_bundle

TARGET = PwGisClientApp
TEMPLATE = app
ProjectDir = $$PWD
INCLUDEPATH += \
	$$ProjectDir

SOURCES += main.cpp\
	mainwindow.cpp \
	Options.cpp \
	TestFormats.cpp \
	TestCaptionAndTooltip.cpp \
	DemoUpdateFeatureParameters.cpp

HEADERS  += mainwindow.h \
	Options.h \
	TestFormats.h \
	TestCaptionAndTooltip.h \
	DemoUpdateFeatureParameters.h

FORMS    += mainwindow.ui \
	TestFormats.ui \
	TestBasicFeatures.ui \
	TestCaptionAndTooltip.ui \
	DemoUpdateFeatureParameters.ui

RESOURCES +=

include(../../specs/pwgiswidget.pri)

# disablePrebuildStep()
