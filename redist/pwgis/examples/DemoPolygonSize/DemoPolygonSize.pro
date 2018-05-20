#-------------------------------------------------
#
# Project created by QtCreator 2013-10-17T16:26:26
#
#-------------------------------------------------

QT       += webkit gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoPolygonSize
TEMPLATE = app


SOURCES += main.cpp\
        DemoPolygonSize.cpp

HEADERS  += DemoPolygonSize.h

FORMS    += DemoPolygonSize.ui

include(../../specs/pwgiswidget.pri)
