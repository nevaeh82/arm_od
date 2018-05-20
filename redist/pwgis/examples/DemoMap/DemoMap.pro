#-------------------------------------------------
#
# Project created by QtCreator 2013-03-14T14:28:44
#
#-------------------------------------------------

QT       += gui core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TRANSLATIONS += DemoMap_en.ts\
		DemoMap_ru.ts\
		DemoMap_de.ts

TARGET = DemoMap
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
	  $$ProjectDir

SOURCES += main.cpp \
	MapWindow.cpp

FORMS += \
	MapWindow.ui

HEADERS += \
	MapWindow.h

RESOURCES += \
	demoMapTranslations.qrc
