#-------------------------------------------------
#
# Project created by QtCreator 2013-10-16T18:00:57
#
#-------------------------------------------------

QT       += webkit gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoObjectsTimer
TEMPLATE = app


SOURCES += main.cpp\
        DemoObjectsTimer.cpp

HEADERS  += DemoObjectsTimer.h

FORMS    += DemoObjectsTimer.ui

include(../../specs/pwgiswidget.pri)
