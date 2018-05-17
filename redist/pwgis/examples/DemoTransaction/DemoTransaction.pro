#-------------------------------------------------
#
# Project created by QtCreator 2013-09-04T18:11:58
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoTransaction
CONFIG   -= app_bundle
ProjectDir = $$PWD

TEMPLATE = app

include( ../../specs/pwgiswidget.pri )

INCLUDEPATH += \
          $$ProjectDir

SOURCES += main.cpp \
    DemoTransaction.cpp

FORMS += \
    DemoTransaction.ui

HEADERS += \
    DemoTransaction.h
